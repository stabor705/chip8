#include "mem.h"

#include <fstream>

uint16_t Memory::fetch_instruction(uint16_t addr) const {
    uint16_t instr = mem[addr];
    instr = instr << 8;
    instr += mem[addr + 1];
    return instr;
}

void Memory::set(uint16_t addr, uint8_t value) {
    mem[addr] = value;
}

uint8_t Memory::get(uint16_t addr) const {
    return mem[addr];
}

void Memory::clear() {
    std::fill(mem.begin(), mem.end(), 0);
}
