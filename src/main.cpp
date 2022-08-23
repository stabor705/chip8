#include "emulation.h"

#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "File not provided" << std::endl;
        return 1;
    }
    Emulation emulation;
    emulation.load_program(argv[1]);
    emulation.run();
    return 0;
}
