#include "controlswindow.h"

#include <imgui.h>

void ControlsWindow::show() {
    ImGui::Begin("Controls");
    if (halted)
        halted = !ImGui::Button("Run");
    else
        halted = ImGui::Button("Halt");
    next_instr = ImGui::Button("Step");
    reset = ImGui::Button("Reset");
    ImGui::End();
}
