#include <iostream>
#include <fstream>
#include <thread>
#include <random>
#include "Memory.h"

using namespace std;

int main() {
//    ofstream logBuf("/var/error_generator.log");
//    streambuf *oldCoutBuf = cout.rdbuf(logBuf.rdbuf());
//    streambuf *oldCerrBuf = cerr.rdbuf(logBuf.rdbuf());

    random_device rd;
    default_random_engine gen(rd());
    uniform_int_distribution<unsigned> distrib(60, 95);
    Memory* memory = nullptr;
    for (int i = 0; i < 10; i++) {
        memory = new Memory(GenerateType::BURST, ErrorType::MEMORY_LEAK,
                            30, distrib(gen));
        memory->generateError();
        delete memory;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        memory = new Memory(GenerateType::STEADY_UP_AND_KEEP, ErrorType::NORMAL,
                            30, distrib(gen));
        memory->generateError();
        delete memory;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
//    cout.rdbuf(oldCoutBuf);
//    cerr.rdbuf(oldCerrBuf);
    return 0;
}