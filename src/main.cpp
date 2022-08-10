#include "emulation.h"

#include <fstream>

int main(int argc, char *argv[]) {
    Emulation emulation;
    std::ifstream file(argv[1]);
    emulation.load_program(file);
    emulation.run();
    return 0;
}
