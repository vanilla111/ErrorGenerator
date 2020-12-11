#include <iostream>
#include <fstream>
#include "Memory.h"

using namespace std;

int main() {
    ofstream logBuf("/var/error_generator.log");
    streambuf *oldCoutBuf = cout.rdbuf(logBuf.rdbuf());
    streambuf *oldCerrBuf = cerr.rdbuf(logBuf.rdbuf());
    std::cout << "Hello, World!" << std::endl;

    cout.rdbuf(oldCoutBuf);
    cerr.rdbuf(oldCerrBuf);
    return 0;
}
