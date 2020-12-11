//
// Created by wang on 2020/12/10.
//

#ifndef ERRORGENERATOR_MEMORY_H
#define ERRORGENERATOR_MEMORY_H

#include <vector>

#include "ErrorType.h"
#include "GenerateType.h"

using namespace std;

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
    // 内存占用增长速率
    float up_rate;
    // 降低速率
    float down_rate;
    // 已分配内存指针容器
    vector<char*> pointer_container;
    // 可使用内存(phy + swap) byte
    int available_mem;
    // 是否使用交换区
    bool use_swap;

public:

    Memory(GenerateType t1, ErrorType t2, int continue_time, int threshold, int burst_times, bool swap);

    ~Memory();

    void generateError();

private:


};

#endif //ERRORGENERATOR_MEMORY_H
