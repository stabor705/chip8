#include "emulation.h"

#include <fstream>

int main(int argc, char *argv[]) {
    Emulation emulation;
    spdlog::set_level(spdlog::level::debug);
    std::ifstream file(argv[1], std::fstream::binary);
    emulation.load_program(file);
    emulation.run();
    return 0;
}