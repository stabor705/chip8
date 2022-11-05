#include "emulation.h"

#include <iostream>

int main(int argc, char *argv[]) {
    Emulation emulation;
    emulation.load_program(argv[1]);
    emulation.run();
    return 0;
}
