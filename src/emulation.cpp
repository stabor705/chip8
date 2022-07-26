#include "emulation.h"

#include <iterator>

void Emulation::run() {
    // TODO: For now program.size() == 2^16 - 0x200 => emulator is ded ;- (
    while (chip.run_program_instr()) {}
}

void Emulation::load_program(std::istream &is) {
    std::vector<uint8_t> program;
    while (!is.eof()) {
        uint8_t byte;
        is.read(reinterpret_cast<char*>(&byte), 1);
        program.push_back(byte);
    }
    chip.load_program(program);
    spdlog::info("Successfully loaded program {} bytes long.", program.size());
}