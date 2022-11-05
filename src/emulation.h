#ifndef CHIP8_EMULATION_H
#define CHIP8_EMULATION_H

#include "chip8/chip8.h"
#include "ui/userinterface.h"

/**
 * @brief A glue that combines everything together.
 *
 * This emulator program is really comprised from 2 subsystems.
 * 1. The chip8 emulation logic represented by Chip8 class. It allows to parse and execute
 * instructions without any knowledge of how input is delivered from user and how output is
 * displayed to user. Its whole interface is programmatic.
 * 2. The user interface represented by UserInterface class - a system that initializes
 * graphics, creates window, processes
 * user input, draws GUI, properly draws game display using pixel matrix provided
 * by Display class.
 *
 * There is a need to tie it all together and run in loop. Most importantly, in order
 * to get good performence, we can't tick these subsystems with same frequency. Program
 * instructions should be run multiple times befere the display is redrawn.
 * Ideally, we would run program instructions until display has beed changed.
 * This, however, would produce cpu-bound emulation speed and would probably run too
 * fast on most computers.
 * Thus, Emulation also takes care of keeping an upper bound on rendered frames
 * per second.
 *
 * Emulation also extracts data from Chip8 and transmits it to UserInterface, taking
 * care of communication between these two.
 *
 * In order to run program, you just have to call Emulation::run method. Rest can be taken
 * care of by GUI.
 */
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
