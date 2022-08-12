#include "emulation.h"

#include <sstream>

const std::unordered_map<int, uint8_t> Emulation::key_bindings= {
    { 49, 0 }, { 50, 1 }, { 51, 2 }, { 52, 3 },
    { 81, 4 }, { 87, 5 }, { 69, 6 }, { 82, 7 },
    { 65, 8 }, { 83, 9 }, { 68, 10 }, { 70, 11 },
    { 90, 12 }, { 88, 13 }, { 67, 14 }, { 86, 15 }
};

void Emulation::run() {
    while (!ui.user_quit()) {
        ui.update_chip_state(chip);
        ui.update_display(chip.get_screen());

        ui.render_frame();
        ui.process_input();
        handle_key(ui.get_key_pressed());
        if (ui.should_run_instr()) {
            try {
                if (!chip.run_program_instr()) {
                    ui.chip_halted();
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

void Emulation::handle_key(int key) {
    if (key == 0) {
        chip.reset_key();
        return;
    }
    auto it = key_bindings.find(key);
    if (it != key_bindings.end()) {
        chip.press_key(it->second);
        std::stringstream ss;
        ss << (int)it->second << " has been pressed";
        ui.add_message(ss.str());
    }
}
