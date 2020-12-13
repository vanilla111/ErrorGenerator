//
// Created by wang on 2020/12/10.
//
#include "Memory.h"
#include "FileHelper.h"
#include <vector>
#include <map>
#include <thread>
#include <iostream>
#include <random>
#include <ctime>
#include <string.h>

using namespace std;

Memory::Memory(GenerateType t1, ErrorType t2, int continue_time,
        int threshold, int burst_times, bool swap) {
    this->generate_type = t1;
    this->error_type = t2;
    // 持续时间至少30s
    this->continue_time = continue_time < 30 ? 30 : continue_time;
    // 阀值最高为10%-95%
    this->threshold = threshold > 95 ? 95 : threshold;
    this->burst_times = burst_times;
    this->use_swap = swap;
    this->i_take = 0;
}

void Memory::generateError() {
    if (!readMemInfo()) {
        this->generate_type = NO_ACTION;
        this->error_type = NORMAL;
    }
    FileHelper::write(error_label_output_path, this->error_type, true);
    switch (this->generate_type) {
        case NO_ACTION:
            noAction(continue_time);
            break;
        case LOW_LEVEL:
            // 维持在20%左右
            lowLevel();
            break;
        case HIGH_LEVEL:
            // 维持在90%左右
            highLevel();
            break;
        case STEADY_UP_AND_KEEP:
            // 达到阀值后继续保持
            steadyUpAndKeep();
            break;
        case STEADY_UP_THEN_DOWN:
            // 达到阀值后再缓慢下降
            steadyUpAndDown();
            break;
        case BURST:
            burst(continue_time);
            break;
        case BURST_WITH_FREQUENCY:
            burstWithFrequency();
            break;
    }
}

Memory::~Memory() {
    // 退出前先将label修改为正常
    FileHelper::write(error_label_output_path, ErrorType::NORMAL, true);
    clearAll();
}

void Memory::noAction(int seconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(seconds * 1000));
}

void Memory::lowLevel() {
    this->threshold = 20;
    keepSteadyLevel();
}

void Memory::highLevel() {
    this->threshold = 90;
    keepSteadyLevel();
}

void Memory::keepSteadyLevel() {
    long except = mem_total * (threshold * 1.0 / 100);
    long used = mem_total - mem_free - mem_buff_cache;
    long malloc_size = -1;
    if (used < except) {
        malloc_size = except - used;
    }
    takeMemAndKeep(malloc_size, continue_time);
}

bool Memory::takeMemAndKeep(long mem_size, int keep_time) {
    bool result = true;
    if (mem_size > 0) {
        char *p = (char *) malloc(mem_size);
        if (p != nullptr) {
            memset(p, 0, mem_size);
            pointer_container.insert({p, mem_size});
            i_take += mem_size;
        } else {
            cerr << "[Memory::takeMemAndKeep]分配内存失败" << endl;
            result = false;
        }
    }
    if (keep_time > 0) {
        noAction(keep_time);
    }
    return result;
}

void Memory::steadyUpAndKeep() {
    // 三段式，保持水平->稳步上升->保持水平
    int used_time = steadyUp();
    if (used_time < continue_time) {
        noAction(continue_time - used_time);
    }
}

void Memory::steadyUpAndDown() {
    // 四段式，保持水平->稳步上升->保持水平->逐步下降
    steadyUp();
}

int Memory::steadyUp() {
    random_device rd;
    default_random_engine gen(rd());
    uniform_int_distribution<unsigned> distrib(1, 3);
    double use_threshold = threshold * 1.0 / 100;
    double free_threshold = 1 - use_threshold;
    time_t start_time = time(nullptr);
    int except_use_time = continue_time * 0.4;
    while ((mem_free + mem_buff_cache) > (long)(mem_total * free_threshold)) {
        // 计算本次循环需要分配的内存
        int used_time = static_cast<int> (time(nullptr) - start_time);
        long except = mem_total * use_threshold;
        long used = mem_total - mem_free - mem_buff_cache;
        if (used_time >= except_use_time) {
            // 直接分配到阀值
            long malloc_size = except - used;
            takeMemAndKeep(malloc_size, distrib(gen));
        } else {
            // 剩余内存 / 剩余时间
            int remain_time = except_use_time - used_time;
            if (except - used > remain_time) {
                long malloc_size = (except - used) / remain_time;
                takeMemAndKeep(malloc_size, 1);
            } else {
                // 这意味着每秒分配1字节不到
                break;
            }
        }
        if (!readMemInfo()) {
            cerr << "[Memory::steadyUp]更新内存信息错误，提前退出" << endl;
            break;
        }
    }
    return static_cast<int> (time(nullptr) - start_time);
}

void Memory::burst(int interval) {
    // 随机选取一个时间点，短时间快速占用内存递增到阀值
    // 维持3秒后迅速降低
    if (interval < 11) return;
    time_t now = time(nullptr);
    random_device rd;
    default_random_engine gen(rd());
    uniform_int_distribution<unsigned> distrib(1, (interval - 10));
    int sleep_time = distrib(gen);
    noAction(sleep_time);
    if (sleep_time > 60) {
        // 如果读取内存信息后超过一分钟，在计算前重新读取
        readMemInfo();
    }
    long except_use = mem_total * (threshold * 1.0 / 100);
    long current_use = mem_total - mem_free - mem_buff_cache;
    long diff = except_use - current_use;
    float take_percent = 0.25;
    int malloc_failed_times = 0;
    if (diff > 0) {
        while (malloc_failed_times < 5 && current_use < except_use) {
            // 保险起见，分配失败五次直接退出
            long malloc_size = diff * take_percent;
            bool res = takeMemAndKeep(malloc_size, 1);
            if (res) {
                current_use += malloc_size;
            } else {
                malloc_failed_times++;
                take_percent -= 0.05;
            }
        }
    }
    noAction(3);
    clearAll();
    int remain = static_cast<int> (time(nullptr) - now);
    if (remain < interval) {
        noAction(interval - remain);
    }
}

void Memory::burstWithFrequency() {
    int interval = continue_time / burst_times;
    if (interval < 12) interval = 12;
    int used_time = 0;
    while (used_time < continue_time) {
        burst(interval);
        used_time += interval;
        if (!readMemInfo()) {
            cerr << "[Memory::burstWithFrequency]按频率突发型占用内存失败" << endl;
            break;
        }
    }
}

void Memory::clearAll() {
    map<char*, long>::iterator iter;
    for (iter = pointer_container.begin(); iter != pointer_container.end(); iter++) {
        free(iter->first);
    }
    pointer_container.clear();
    i_take = 0;
}

bool Memory::readMemInfo() {
    int max_retry_times = 5;
    vector<long> mem_info;
    for (int i = 0; i < max_retry_times; i++) {
        FileHelper::readProcMemInfo(mem_info);
        if (mem_info.empty()) {
            if (i == (max_retry_times - 1)) {
                cerr << "[Memory::generateError]读取内存使用数据失败" << endl;
                return false;
            }
        } else {
            break;
        }
    }
    this->mem_total = mem_info[0];
    this->mem_free = mem_info[1];
    this->mem_buff_cache = mem_info[2] + mem_info[3];
    this->swap_total = mem_info[4];
    this->swap_free = mem_info[5];
    if (use_swap) {
        this->mem_total += this->swap_total;
        this->mem_free += this->swap_free;
    }
    return true;
}
