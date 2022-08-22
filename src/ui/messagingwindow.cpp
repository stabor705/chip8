#include "messagingwindow.h"

#include <imgui.h>
#include <string>

void MessagingWindow::show(float height, float width) {
    ImVec2 size(height, width);
    ImGui::BeginChild("Messages", size, true, 0);
    for (const auto &msg : messages) {
        ImGui::Text("%s", msg.c_str());
    }
    ImGui::EndChild();
}

void MessagingWindow::add_message(const std::string &msg) {
    messages.push_back(msg);
}
