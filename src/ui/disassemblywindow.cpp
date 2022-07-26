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

void DisassemblyWindow::show(float width, float height) const {
    ImGui::BeginChild("Disassembly Window", ImVec2(width, height), false, 0);
    ImGui::BeginChild("Label", ImVec2(0, ImGui::GetTextLineHeightWithSpacing()), false, 0);
    ImGui::Text("%s", "Disassembly:");
    ImGui::EndChild();
    ImGui::BeginChild("Disassembly", ImVec2(0, 0), true, 0);
    for (int i = 0; i < disassembly.size(); i++) {
        if (disassembly[i].empty())
            continue;
        ImGui::Text("%x: %s %s", starting_addr + i * 2, disassembly[i].c_str(),
                    (i * 2 + starting_addr == pc) ? "<--" : "");
        if (i * 2 + starting_addr == pc)
            ImGui::SetScrollHereY(0);
    }
    ImGui::EndChild();
    ImGui::EndChild();
}
