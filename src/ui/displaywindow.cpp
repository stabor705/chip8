#include "displaywindow.h"

#include <glad/glad.h>
#include <imgui.h>

void DisplayWindow::initialize() {
    glGenTextures(1, &texture);
}

void DisplayWindow::show(float width, float height) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::BeginChild("Display", ImVec2(width, height), true);
    ImGui::Image((void*)(intptr_t)texture, ImVec2(width, height));
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void DisplayWindow::update(const Pixels &pixels) {
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            unsigned char value = pixels[y][x] ? 255 : 0;
            for (int i = 0; i < 3; i++)
                data[y][x][i] = value;
        }
    }
}
