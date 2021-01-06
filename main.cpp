#include <cstdlib>
#include "Memory.h"
#include "FileHelper.h"

using namespace std;

void exitHandler() {
    FileHelper::write(error_label_output_path, ErrorType::NORMAL, true);
}

int main() {

    atexit(exitHandler);

    Memory* memory = new Memory(GenerateType::CONSTANT_TAKE, ErrorType::MEMORY_LEAK);
    memory->generateError();
    delete memory;

    return 0;
}