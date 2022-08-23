#include "emulation.h"

#include <thread>
#include <fstream>
#include <sstream>

void Emulation::run() {
    while (!ui.user_quit()) {
        auto start = Clock::now();

        ui.update_chip_state(chip);

        ui.show();
        ui.process_input();
        handle_key(ui.get_key_pressed());
        if (ui.file_should_be_loaded())
            load_program(ui.get_file_to_load());

        if (ui.should_reset_chip())
            chip.reset();
        try {
            if (ui.should_run()) {
                while (
                       !chip.display_changed() && chip.run_program_instr() &&
                       frame_time >= Clock::now() - start
                       );
            } else if (ui.should_step()) {
                chip.run_program_instr();
            }
        }
        catch (std::exception &e) {
            ui.add_message(e.what());
        }
        std::this_thread::sleep_for(frame_time - (Clock::now() - start));
    }
}

void Emulation::load_program(fs::path filepath) {
    std::vector<uint8_t> program;
    std::ifstream file(filepath);
    while (!file.eof()) {
        uint8_t byte;
        file.read(reinterpret_cast<char*>(&byte), 1);
        program.push_back(byte);
    }
    chip.reset();
    chip.load_program(program);
    ui.run_disassembler(program);
    ui.set_file_loaded(filepath);
    std::stringstream ss;
    ss << "Successfully loaded file " << filepath;
    ui.add_message(ss.str());
}

void Emulation::handle_key(int key) {
    if (key == -1) {
        chip.release_key();
        return;
    }
    chip.press_key(key);
}
