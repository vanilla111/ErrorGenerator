//
// Created by wang on 2020/12/10.
//

#ifndef ERRORGENERATOR_ERRORTYPE_H
#define ERRORGENERATOR_ERRORTYPE_H

enum ErrorType {
    NORMAL, MEMORY_LEAK, MEM_ANOMAL_WITH_CPU, MEM_ANOMAL_WITH_NETWORK, MEM_ANOMAL_WITH_DISK, SERIOUS
};

static const char* error_label_output_path = "/etc/collector/label.txt";

#endif //ERRORGENERATOR_ERRORTYPE_H
