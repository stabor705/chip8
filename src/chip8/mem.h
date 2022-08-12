#ifndef CHIP8_MEM_H
#define CHIP8_MEM_H

#include <array>
#include <cstddef>
#include <cstdint>

template<size_t N>
class Memory {
public:
    Memory() = default;

    uint16_t fetch_instruction(uint16_t addr) const;
    void set(uint16_t addr, uint8_t value);
    uint8_t get(uint16_t addr) const;
    void clear();
private:
    std::array<uint8_t , N> mem;
};

template <size_t N>
uint16_t Memory<N>::fetch_instruction(uint16_t addr) const {
    uint16_t instr = mem[addr];
    instr = instr << 8;
    instr += mem[addr + 1];
    return instr;
}

template <size_t N>
void Memory<N>::set(uint16_t addr, uint8_t value) {
    mem[addr] = value;
}

template <size_t N>
uint8_t Memory<N>::get(uint16_t addr) const {
    return mem[addr];
}

template <size_t N>
void Memory<N>::clear() {
    std::fill(mem.begin(), mem.end(), 0);
}

#endif //CHIP8_MEM_H
