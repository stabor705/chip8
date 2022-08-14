#include "emulation.h"

#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "File not provided" << std::endl;
        return 1;
    }
    std::ifstream file(argv[1]);
    if (file.bad()) {
        std::cerr << "Could not open file " << argv[1] << std::endl;
        return 1;
    }
    Emulation emulation;
    emulation.load_program(file);
    emulation.run();
    return 0;
}
