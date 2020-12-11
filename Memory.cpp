//
// Created by wang on 2020/12/10.
//
#include "Memory.h"
#include "FileHelper.h"

Memory::Memory(GenerateType t1, ErrorType t2, int continue_time, int threshold, int burst_times, bool swap) {
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

}

Memory::~Memory() {

}
