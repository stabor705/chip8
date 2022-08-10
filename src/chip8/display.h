#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <cstdint>
#include <array>
#include <vector>

class Display {
public:
    typedef std::array<std::array<bool, 64>, 32> Pixels;

    bool draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite);
    const Pixels &get_pixels() const { return pixels; }
    void clear();
private:
    Pixels pixels;
};

#endif //CHIP8_DISPLAY_H
