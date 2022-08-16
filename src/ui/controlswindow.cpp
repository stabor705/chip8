#include "controlswindow.h"

void ControlsWindow::show() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(32, 0));
    ImGui::BeginChild("Controls", ImVec2(8 * 32, 2 * 32), true, 0);

    //TODO: That's a pretty ugly way to do it
    ImFont *font = ImGui::GetFont();
    float scale = 1.5f;
    font->Scale *= scale;
    ImGui::PushFont(font);

    if (halted) {
        ImGui::PushStyleColor(ImGuiCol_Button, run_button_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, run_button_color_hovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, run_button_color_clicked);
        halted = !ImGui::Button("Run", ImVec2(48, 32));
        ImGui::PopStyleColor(3);
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Button, halt_button_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, halt_button_color_hovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, halt_button_color_clicked);
        halted = ImGui::Button("Halt", ImVec2(48, 32));
        ImGui::PopStyleColor(3);
    }

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, step_button_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, step_button_color_hovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, step_button_color_clicked);
    next_instr = ImGui::Button("Step", ImVec2(48, 32));
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    reset = ImGui::Button("Reset", ImVec2(64, 32));

    ImGui::GetFont()->Scale /= scale;
    ImGui::PopFont();
    ImGui::PopStyleVar(2);
    ImGui::EndChild();
}
