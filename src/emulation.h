#ifndef CHIP8_EMULATION_H
#define CHIP8_EMULATION_H

#include "chip8/chip8.h"
#include "ui/userinterface.h"

class Emulation {
public:
    void run();
    void load_program(const fs::path &filepath);
private:
    static auto constexpr frame_time = chrono::milliseconds(1000 / 24);
    static int constexpr instr_per_frame = 60;
    using Clock = chrono::steady_clock;

    void handle_key(int key);

    Chip8 chip;
    UserInterface ui;
};


#endif //CHIP8_EMULATION_H
