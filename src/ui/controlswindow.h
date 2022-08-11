#ifndef CONTROLSWINDOW_H
#define CONTROLSWINDOW_H


class ControlsWindow {
public:
    ControlsWindow() : halted(true) {}

    void show();

    bool is_halted() const { return halted; }
    bool should_run_next_frame() const { return next_frame; }
public:
    bool halted;
    bool next_frame {};
};

#endif // CONTROLSWINDOW_H
