#include "messagingwindow.h"

#include <imgui.h>

void MessagingWindow::show() const {
    ImGui::Begin("Messages");
    for (const auto &msg : messages) {
        ImGui::Text("%s", msg.c_str());
    }
    ImGui::End();
}

void MessagingWindow::add_message(const std::string &msg) {
    messages.push_back(msg);
}
