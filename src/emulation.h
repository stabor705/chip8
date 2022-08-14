#ifndef CHIP8_EMULATION_H
#define CHIP8_EMULATION_H

#include "chip8/chip8.h"
#include "ui/userinterface.h"

#include <ios>
#include <unordered_map>

class Emulation {
public:
    void run();
    void load_program(std::istream &is);
private:
    static const std::unordered_map<int, uint8_t> key_bindings;

    void handle_key(int key);

    Chip8 chip;
    UserInterface ui;
};


#endif //CHIP8_EMULATION_H
