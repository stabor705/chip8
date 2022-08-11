#ifndef DISASSEMBLYWINDOW_H
#define DISASSEMBLYWINDOW_H

#include <vector>
#include <string>
#include <cstdint>

class DisassemblyWindow {
public:
    DisassemblyWindow() : pc(0x200) {}
    void load_program(const std::vector<uint8_t> &program);
    void show() const;
    void update_pc(unsigned int pc) { this->pc = pc; }

private:
    std::vector<std::string> disassembly;
    unsigned int pc;
};

#endif // DISASSEMBLYWINDOW_H
