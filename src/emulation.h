#ifndef CHIP8_EMULATION_H
#define CHIP8_EMULATION_H

#include "chip8/chip8.h"
#include "ui/userinterface.h"

#include <ios>

class Emulation {
public:
    Emulation() = default;
    void run();
    void load_program(std::istream &is);
private:
    Chip8 chip;
    UserInterface ui;
};


#endif //CHIP8_EMULATION_H
