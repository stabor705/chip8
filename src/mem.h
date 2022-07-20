#ifndef CHIP8_MEM_H
#define CHIP8_MEM_H

#include <array>
#include <cstdint>
#include <ios>

class Memory {
public:
    Memory() = default;

    void load_program(std::istream &is, uint16_t offset);
    uint16_t fetch_instruction(uint16_t addr) const;
    void set(uint16_t addr, uint8_t value);
    uint8_t get(uint16_t addr) const;
private:
    static constexpr std::size_t MEM_SIZE = 0xFFFF;

    std::array<uint8_t , MEM_SIZE> mem;
};


#endif //CHIP8_MEM_H
