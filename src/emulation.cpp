#include "emulation.h"

#include <iterator>

void Emulation::run() {
    while (chip.run_program_instr()) {}
}

void Emulation::load_program(std::istream &is) {
    std::istream_iterator<uint8_t> start(is), end;
    std::vector<uint8_t> program(start, end);
    chip.load_program(program);
}