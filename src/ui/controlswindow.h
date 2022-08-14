#ifndef CONTROLSWINDOW_H
#define CONTROLSWINDOW_H


class ControlsWindow {
public:
    ControlsWindow() : halted(true) {}

    void show();
    void halt() { halted = true; }

    bool is_halted() const { return halted; }
    bool should_step() const { return next_instr; }
    bool should_reset() const { return reset; }
public:
    bool halted;
    bool next_instr {};
    bool reset {};
};

#endif // CONTROLSWINDOW_H
