#ifndef KEYBOARDWINDOW_H
#define KEYBOARDWINDOW_H

#include <imgui.h>

#include <array>

class KeyboardWindow {
public:
    void show(float width, float height);
    int get_key() const;
private:
    static constexpr ImVec4 key_button_color{0.48f, 0.27f, 0.58f, 1};
    static constexpr ImVec4 key_button_color_hovered{0.7f, 0.44f, 0.64f, 1};
    static constexpr ImVec4 key_button_color_clicked{0.27f, 0.07f, 0.38f, 1};

    std::array<bool, 16> keyboard {};
};

#endif // KEYBOARDWINDOW_H
