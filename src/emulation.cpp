#include "emulation.h"

#include <iterator>

void Emulation::run() {
    while (!ui.user_quit()) {
        ui.update_chip_state(chip);

        ui.render_frame();
        ui.process_input();
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
