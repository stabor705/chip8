#ifndef CONTROLSWINDOW_H
#define CONTROLSWINDOW_H


class ControlsWindow {
public:
    ControlsWindow() : halted(true) {}

    void show();
    void halt() { halted = true; }

    bool is_halted() const { return halted; }
    bool should_run_next_frame() const { return next_frame; }
    bool should_reset() const { return reset; }
public:
    bool halted;
    bool next_frame {};
    bool reset {};
};

#endif // CONTROLSWINDOW_H
