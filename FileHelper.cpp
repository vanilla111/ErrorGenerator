//
// Created by wang on 2021/1/2.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "FileHelper.h"

using namespace std;

void FileHelper::write(const char* file_name, ErrorType content, bool overwrite) {
    ios::openmode open_mode = fstream::out;
    if (overwrite) {
        open_mode |= fstream::trunc;
    } else {
        open_mode |= fstream::app;
    }
    fstream fop(file_name, open_mode);
    if (!fop.is_open()) {
        cerr << "[FileHelper::write]打开文件失败: " << file_name << endl;
        return;
    }
    fop << content << endl;
    fop.flush();
    fop.close();
}

void FileHelper::readProcMemInfo(vector<long> &result) {
    result.clear();
    FILE *file = fopen("/proc/meminfo","r");
    if (file == nullptr) {
        cerr << "[FileHelper::readProcMemInfo]文件打开失败" << endl;
        return;
    }
    char tmp[50];
    fscanf(file, "MemTotal: %s kB\n", tmp);
    long mem_total = atol(tmp) * 1024;
    fscanf(file, "MemFree: %s kB\n", tmp);
    long mem_free = atol(tmp) * 1024;
    fscanf(file, "%s", tmp);
    fscanf(file, "%s kB\n", tmp);
    fscanf(file, "Buffers: %s kB\n", tmp);
    long mem_buff = atol(tmp) * 1024;
    fscanf(file, "Cached: %s kB\n", tmp);
    long mem_cache = atol(tmp) * 1024;
    for (int i = 0; i < 9; ++i) {
        fscanf(file, "%s", tmp);
        fscanf(file, "%s kB\n", tmp);
    }
    fscanf(file, "SwapTotal: %s kB\n", tmp);
    long swap_total = atol(tmp) * 1024;
    fscanf(file, "SwapFree: %s kB\n", tmp);
    long swap_free = atol(tmp) * 1024;
    result.push_back(mem_total);
    result.push_back(mem_free);
    result.push_back(mem_buff);
    result.push_back(mem_cache);
    result.push_back(swap_total);
    result.push_back(swap_free);
    fclose(file);
}