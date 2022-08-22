#ifndef DISASSEMBLYWINDOW_H
#define DISASSEMBLYWINDOW_H

#include <vector>
#include <string>
#include <cstdint>

class DisassemblyWindow {
public:
    DisassemblyWindow(unsigned int starting_addr) : pc(starting_addr),
    starting_addr(starting_addr) {}
    void load_program(const std::vector<uint8_t> &program);
    void show(float width, float height) const;
    void update_pc(unsigned int pc) { this->pc = pc; }

private:
    std::vector<std::string> disassembly;
    unsigned int pc;
    unsigned int starting_addr;
};

#endif // DISASSEMBLYWINDOW_H
