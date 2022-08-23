#ifndef CHIP8_EMULATION_H
#define CHIP8_EMULATION_H

#include "chip8/chip8.h"
#include "ui/userinterface.h"

#include <ios>
#include <unordered_map>

class Emulation {
public:
    void run();
    void load_program(fs::path filepath);
private:
    static const std::unordered_map<int, uint8_t> key_bindings;
    static auto constexpr frame_time = chrono::milliseconds(1000 / 24);
    using Clock = chrono::steady_clock;

    void handle_key(int key);

    Chip8 chip;
    UserInterface ui;
};


#endif //CHIP8_EMULATION_H
