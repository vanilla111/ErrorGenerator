//
// Created by wang on 2020/12/10.
//

#ifndef ERRORGENERATOR_FILEOPERATION_H
#define ERRORGENERATOR_FILEOPERATION_H

#include <string>
#include <vector>

using namespace std;

class FileHelper {
public:
    template <typename T>
    static void write(const std::string &file_name, T content, bool overwrite);

    static void readProcMemInfo(vector<long> &result);
};

#endif //ERRORGENERATOR_FILEOPERATION_H
