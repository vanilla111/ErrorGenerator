//
// Created by wang on 2020/12/10.
//

#ifndef ERRORGENERATOR_FILEOPERATION_H
#define ERRORGENERATOR_FILEOPERATION_H

#include "ErrorType.h"

class FileHelper {
public:
    static void write(const char* file_name, ErrorType content, bool overwrite);

    static void readProcMemInfo(std::vector<long> &result);

private:
    FileHelper() {};
};

#endif //ERRORGENERATOR_FILEOPERATION_H
