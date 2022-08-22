#include "keyboardwindow.h"

#include <sstream>

void KeyboardWindow::show(float width, float height) {
    ImGui::PushStyleColor(ImGuiCol_Button, key_button_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, key_button_color_hovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, key_button_color_clicked);
    ImGui::BeginChild("Keyboard", ImVec2(width, height), true, 0);
    float unit_w = ImGui::GetWindowWidth() / 8;
    float unit_h = ImGui::GetWindowHeight()/ 8;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(unit_w * 0.5f, unit_h * 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(unit_w * 0.5f, unit_h * 0.5f));
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int idx = y * 4 + x;
            std::stringstream ss;
            ss << std::hex << idx;
            ImGui::Button(ss.str().c_str(), ImVec2(unit_w * 1.5f, unit_h * 1.5f));
            keyboard[idx] = ImGui::IsItemActive();
            if (x != 3)
                ImGui::SameLine();
        }
    }
    ImGui::PopStyleVar(2);
    ImGui::EndChild();
    ImGui::PopStyleColor(3);
}

int KeyboardWindow::get_key() const {
    for (int i = 0; i < 16; i++)
        if (keyboard[i])
            return i;
    return -1;
}
