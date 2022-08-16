#include "messagingwindow.h"

#include <imgui.h>
#include <string>

void MessagingWindow::show() {
    ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
    ImVec2 size(24 * 32 + item_spacing.x, 10 * 32 + item_spacing.y);
    ImGui::BeginChild("Messages", size, true, 0);
    for (const auto &msg : messages) {
        ImGui::Text("%s", msg.c_str());
    }
    ImGui::EndChild();
}

void MessagingWindow::add_message(const std::string &msg) {
    messages.push_back(msg);
}
