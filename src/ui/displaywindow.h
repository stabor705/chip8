#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <array>

constexpr unsigned int DISPLAY_WIDTH = 64;
constexpr unsigned int DISPLAY_HEIGHT = 32;
typedef std::array<std::array<bool, 64>, 32> Pixels;

class DisplayWindow {
public:
    void initialize();

    void show(float width, float height);
    void update(const Pixels &pixels);
private:
    unsigned int texture {};
    unsigned char data[DISPLAY_HEIGHT][DISPLAY_WIDTH][3] {};
};

#endif // DISPLAYWINDOW_H
