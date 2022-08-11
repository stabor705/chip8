#include "controlswindow.h"

#include <imgui.h>

void ControlsWindow::show() {
    ImGui::Begin("Controls");
    if (halted)
        halted = !ImGui::Button("Run");
    else
        halted = ImGui::Button("Halt");
    next_frame = ImGui::Button("Next frame");
    ImGui::End();
}
