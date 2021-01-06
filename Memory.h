//
// Created by wang on 2020/12/10.
//

#ifndef ERRORGENERATOR_MEMORY_H
#define ERRORGENERATOR_MEMORY_H

#include <vector>
#include <map>

#include "ErrorType.h"
#include "GenerateType.h"

class Memory {
private:
    // 占用内存阀值
    int threshold;
    // 占用总时长 秒
    int continue_time;
    // 生成类型
    GenerateType generate_type;
    // 错误类型
    ErrorType error_type;
    // 如果是按频率生成，计算频次
    int burst_times;
    // 已分配内存指针容器
    std::map<char*, long> pointer_container;
    // 是否使用交换区
    bool use_swap;
    // phy + (swap if use swap)
    long mem_total;
    long mem_free;
    long mem_buff_cache;
    long swap_total;
    long swap_free;
    long i_take;

public:

    Memory(GenerateType t1, ErrorType t2, int continue_time = 60,
           int threshold = 60, int burst_times = 1, bool swap = false);

    ~Memory();

    void generateError();

    void useSwap(bool flag);

    void setContinueTime(int time);

    void setThreshold(int hold);

private:

    static void noAction(int seconds);

    void constantTake();

    void lowLevel();

    void highLevel();

    void keepSteadyLevel();

    bool takeMemAndKeep(long mem_size, int keep_time);

    void steadyUpAndKeep();

    void steadyUpAndDown();

    int steadyUp(int except_use_time);

    void burst(int interval);

    void burstWithFrequency();

    void clearAll();

    bool readMemInfo();

};

#endif //ERRORGENERATOR_MEMORY_H
