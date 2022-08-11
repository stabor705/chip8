#include "emulation.h"

#include <iostream>
#include <thread>

void Emulation::run() {
    while (!ui.user_quit()) {
        ui.update_chip_state(chip);

        ui.update_display(chip.get_screen());

        ui.render_frame();
        ui.process_input();
        if (ui.should_run_instr()) {
            try {
                if (!chip.run_program_instr()) {
                    ui.halt_controls();
                }
            }
            catch (std::exception &e) {
                ui.add_message(e.what());
            }
        }
    }
}

void Emulation::load_program(std::istream &is) {
    std::vector<uint8_t> program;
    while (!is.eof()) {
        uint8_t byte;
        is.read(reinterpret_cast<char*>(&byte), 1);
        program.push_back(byte);
    }
    chip.load_program(program);
    ui.run_disassembler(program);
}
