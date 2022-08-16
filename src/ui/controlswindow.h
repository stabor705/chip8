#ifndef CONTROLSWINDOW_H
#define CONTROLSWINDOW_H

#include <imgui.h>

class ControlsWindow {
public:
    ControlsWindow() : halted(true) {}

    void show();
    void halt() { halted = true; }

    bool is_halted() const { return halted; }
    bool should_step() const { return next_instr; }
    bool should_reset() const { return reset; }
private:
    static constexpr ImVec4 run_button_color{0.17f, 0.48f, 0.04f, 1};
    static constexpr ImVec4 run_button_color_hovered{0.36f, 0.7f, 0.09f, 1};
    static constexpr ImVec4 run_button_color_clicked{0.49f, 0.81f, 0.07f, 1};
    static constexpr ImVec4 step_button_color{0.92f, 0.36f, 0.17f, 1};
    static constexpr ImVec4 step_button_color_hovered{1, 0.5f, 0.25f, 1};
    static constexpr ImVec4 step_button_color_clicked{0.83f, 0.85f, 0.15f, 1};
    static constexpr ImVec4 halt_button_color{0.76f, 0.06f, 0.06f, 1};
    static constexpr ImVec4 halt_button_color_hovered{0.9f, 0.28f, 0.28f, 1};
    static constexpr ImVec4 halt_button_color_clicked{0.39f, 0.0f, 0.0f, 1};


    bool halted;
    bool next_instr {};
    bool reset {};
};

#endif // CONTROLSWINDOW_H
