//
// Created by wang on 2020/12/11.
//
#include "FileHelper.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
void FileHelper::write(const std::string &file_name, T content, bool overwrite) {
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

void FileHelper::readProcMemInfo(vector<int> &result) {
    FILE *file = fopen("/proc/meminfo","r");
    if (file == nullptr) {
        cerr << "[FileHelper::readProcMemInfo]文件打开失败" << endl;
        result.clear();
    }
    char tmp[50];
    fscanf(file, "MemTotal: %s kB\n", tmp);
    int mem_total = atol(tmp);
    fscanf(file, "MemFree: %s kB\n", tmp);
    int mem_free = atol(tmp);
    fscanf(file, "%s", tmp);
    fscanf(file, "%s kB\n", tmp);
    fscanf(file, "Buffers: %s kB\n", tmp);
    int mem_buff = atol(tmp);
    fscanf(file, "Cached: %s kB\n", tmp);
    int mem_cache = atol(tmp);
    for (int i = 0; i < 9; ++i) {
        fscanf(file, "%s", tmp);
        fscanf(file, "%s kB\n", tmp);
    }
    fscanf(file, "SwapTotal: %s kB\n", tmp);
    int swap_total = atol(tmp);
    fscanf(file, "SwapFree: %s kB\n", tmp);
    int swap_free = atol(tmp);
    result.push_back(mem_total);
    result.push_back(mem_free);
    result.push_back(mem_buff);
    result.push_back(mem_cache);
    result.push_back(swap_total);
    result.push_back(swap_free);
    fclose(file);
}
