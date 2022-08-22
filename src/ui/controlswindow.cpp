#include "controlswindow.h"

void ControlsWindow::show(float width, float height) {
    float unit_w = width / 8;
    float unit_h = height / 2;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(unit_w * 0.5f, unit_h * 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(unit_w, 0));
    ImGui::BeginChild("Controls", ImVec2(width, height), true, 0);

    //TODO: That's a pretty ugly way to do it
    ImFont *font = ImGui::GetFont();
    float scale = 1.5f;
    font->Scale *= scale;
    ImGui::PushFont(font);

    if (halted) {
        ImGui::PushStyleColor(ImGuiCol_Button, run_button_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, run_button_color_hovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, run_button_color_clicked);
        halted = !ImGui::Button("Run", ImVec2(unit_w * 1.5f, unit_h));
        ImGui::PopStyleColor(3);
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Button, halt_button_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, halt_button_color_hovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, halt_button_color_clicked);
        halted = ImGui::Button("Halt", ImVec2(unit_w * 1.5f, unit_h));
        ImGui::PopStyleColor(3);
    }

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, step_button_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, step_button_color_hovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, step_button_color_clicked);
    next_instr = ImGui::Button("Step", ImVec2(unit_w * 1.5f, unit_h));
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    reset = ImGui::Button("Reset", ImVec2(unit_w * 2.0f, unit_h));

    ImGui::GetFont()->Scale /= scale;
    ImGui::PopFont();
    ImGui::PopStyleVar(2);
    ImGui::EndChild();
}
