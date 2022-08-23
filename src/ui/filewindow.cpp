#include "filewindow.h"

#include <imgui.h>
#include <ImGuiFileDialog.h>

void FileWindow::show(float width, float height) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::BeginChild("File Window", ImVec2(width, height), true);
    float unit_w = ImGui::GetWindowWidth() / 32;
    float unit_h = ImGui::GetWindowHeight() / 2;
    if (ImGui::Button("Open File",  ImVec2(unit_w * 8, unit_h * 2))) {
        ImGui::SetNextWindowSize(ImVec2(720, 480));
        ImGuiFileDialog::Instance()->OpenDialog("FileDialogKey", "Choose file", ".ch8,.rom", ".");
    }
    if (ImGuiFileDialog::Instance()->Display("FileDialogKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            filepath = ImGuiFileDialog::Instance()->GetFilePathName();
            file_loaded = true;
        }
        ImGuiFileDialog::Instance()->Close();
    }
    ImGui::SameLine();
    if (!filepath.empty())
        ImGui::Text("Currently loaded rom: %s", filepath.filename().c_str());
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

bool FileWindow::new_file() {
    if (file_loaded) {
        file_loaded = !file_loaded;
        return true;
    }
    return false;
}
