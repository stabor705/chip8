#include "keyboardwindow.h"

#include <sstream>

void KeyboardWindow::show() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
    ImGui::PushStyleColor(ImGuiCol_Button, key_button_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, key_button_color_hovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, key_button_color_clicked);
    ImGui::BeginChild("Keyboard", ImVec2(8 * 32, 8 * 32), true, 0);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int idx = y * 4 + x;
            std::stringstream ss;
            ss << std::hex << idx;
            ImGui::Button(ss.str().c_str(), ImVec2(48, 48));
            keyboard[idx] = ImGui::IsItemActive();
            if (x != 3)
                ImGui::SameLine();
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
}

int KeyboardWindow::get_key() const {
    for (int i = 0; i < 16; i++)
        if (keyboard[i])
            return i;
    return -1;
}
