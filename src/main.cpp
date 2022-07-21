#include "emulation.h"

#include <fstream>

int main() {
    Emulation emulation;
    std::ifstream file("test_opcode.ch8", std::fstream::binary);
    emulation.load_program(file);
    emulation.run();
    return 0;
}