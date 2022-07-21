#include "emulation.h"

#include <iterator>
#include <iostream>

void Emulation::run() {
    // TODO: For now program.size() == 2^16 - 0x200 => emulator is ded ;- (
    while (chip.run_program_instr()) {}
}

void Emulation::load_program(std::istream &is) {
    std::istream_iterator<uint8_t> start(is), end;
    std::vector<uint8_t> program(start, end);
    chip.load_program(program);
    spdlog::info("Successfully loaded program {} bytes long.", program.size());
}