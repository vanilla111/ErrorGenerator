#include <iostream>
#include <fstream>
#include "Memory.h"

using namespace std;

int main() {
    ofstream logBuf("/var/error_generator.log");
    streambuf *oldCoutBuf = cout.rdbuf(logBuf.rdbuf());
    streambuf *oldCerrBuf = cerr.rdbuf(logBuf.rdbuf());

    auto* memory = new Memory(GenerateType::HIGH_LEVEL, ErrorType::NORMAL, 120);
    memory->generateError();
    cout.rdbuf(oldCoutBuf);
    cerr.rdbuf(oldCerrBuf);
    return 0;
}