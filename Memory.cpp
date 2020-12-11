//
// Created by wang on 2020/12/10.
//
#include "Memory.h"
#include "FileHelper.h"
#include <vector>
#include <thread>
#include <iostream>

using namespace std;

Memory::Memory(GenerateType t1, ErrorType t2, int continue_time,
        int threshold, int burst_times, bool swap) {
    this->generate_type = t1;
    this->error_type = t2;
    // 持续时间至少一分钟
    this->continue_time = continue_time < 60 ? 60 : continue_time;
    // 阀值最高为10-150%
    this->threshold = threshold;
    this->burst_times = burst_times;
    this->use_swap = swap;
}

void Memory::generateError() {
    vector<long> mem_info;
    FileHelper::readProcMemInfo(mem_info);
    if (mem_info.empty()) {
        cerr << "[Memory::generateError]读取内存使用数据失败" << endl;
        return;
    }
    FileHelper::write(error_label_output_path, this->error_type, true);
    this->mem_total = mem_info[0];
    this->mem_free = mem_info[1];
    this->mem_buff_cache = mem_info[2] + mem_info[3];
    this->swap_total = mem_info[4];
    this->swap_free = mem_info[5];
    switch (this->generate_type) {
        case NO_ACTION:
            noAction();
            break;
        case LOW_LEVEL:
            // 维持在20%左右
            lowLevel();
            break;
        case HIGH_LEVEL:
            // 维持在80%左右
            highLevel();
            break;
        case STEADY_UP_AND_KEEP:
            // 达到阀值后继续保持
            break;
        case STEADY_UP_THEN_DOWN:
            // 达到阀值后再缓慢下降
            break;
        case BURST:
            break;
        case BURST_WITH_FREQUENCY:
            break;
    }
}

Memory::~Memory() {
    for (char* p: pointer_container) {
        free(p);
    }
}

void Memory::noAction() {
    std::this_thread::sleep_for(std::chrono::minutes(this->continue_time / 60 + 1));
}

void Memory::lowLevel() {
    this->threshold = 20;
    takeMemAndKeep();
}

void Memory::highLevel() {
    this->threshold = 80;
    takeMemAndKeep();
}

void Memory::takeMemAndKeep() {
    assert(threshold > 0 && threshold < 100);
    long except = mem_total * (threshold * 1.0 / 100);
    long used = mem_total - mem_free - mem_buff_cache;
    if (used < except) {
        int malloc_size = except - used;
        char *p = (char*)malloc(malloc_size);
        memset(p, 0, malloc_size);
        pointer_container.push_back(p);
    }
    noAction();
}
