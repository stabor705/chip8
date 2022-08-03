#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include "sdlutils.h"

#include <cstdint>
#include <array>
#include <vector>

class Display {
public:
    Display() = default;

    bool draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite);
    bool get_pixel(uint8_t x, uint8_t y) const { return matrix[y][x]; }
    void clear();

private:
    std::array<std::array<bool , 64>, 32> matrix;
};

#endif //CHIP8_DISPLAY_H