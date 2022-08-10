#ifndef DISASSEMBLYWINDOW_H
#define DISASSEMBLYWINDOW_H

#include <vector>
#include <string>
#include <cstdint>

class DisassemblyWindow {
public:
    void load_program(const std::vector<uint8_t> &program);
    void show() const;

private:
    std::vector<std::string> disassembly;
};

#endif // DISASSEMBLYWINDOW_H
