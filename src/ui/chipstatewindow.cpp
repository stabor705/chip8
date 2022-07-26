#include "chipstatewindow.h"

#include <imgui.h>

void ChipStateWindow::update(const Chip8 &chip) {
    for (int i = 0; i < 16; i++) {
        v[i] = chip.get_v(i);
    }
    vi = chip.get_vi();
    pc = chip.get_pc();
    key = chip.get_pressed_key();
}

void ChipStateWindow::show(float width, float height) const {
    ImGui::BeginChild("Chip State Window", ImVec2(width, height), false, 0);
    ImGui::BeginChild("Label", ImVec2(0, ImGui::GetTextLineHeightWithSpacing()));
    ImGui::Text("%s", "Chip state:");
    ImGui::EndChild();
    ImGui::BeginChild("Chip State", ImVec2(0, 0), true);
    ImGui::Text("PC: 0x%x", pc);
    for (int i = 0; i < 16; i++) {
        ImGui::Text("V%X: 0x%x", i, v[i]);
    }
    ImGui::Text("VI: 0x%x", vi);
    ImGui::Text("Key: %x", key);
    ImGui::EndChild();
    ImGui::EndChild();
}
