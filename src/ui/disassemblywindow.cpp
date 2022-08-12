#include "disassemblywindow.h"
#include "chip8/disassembler.h"

#include <imgui.h>

void DisassemblyWindow::load_program(const std::vector<uint8_t> &program) {
    Disassembler disassembler;
    for (int i = 0; i < program.size() - 1; i += 2) {
        uint16_t instr = program[i];
        instr = instr << 8;
        instr += program[i + 1];
        disassembly.push_back(disassembler.disassemble(instr));
    }
}

void DisassemblyWindow::show() const {
    ImGui::Begin("Disassembly");
    for (int i = 0; i < disassembly.size(); i++) {
        ImGui::Text("%x: %s %s", starting_addr + i * 2, disassembly[i].c_str(),
                    (i * 2 + starting_addr == pc) ? "<--" : "");
        if (i * 2 + starting_addr == pc)
            ImGui::SetScrollHereY(0);
    }
    ImGui::End();
}
