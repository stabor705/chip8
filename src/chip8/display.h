#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <cstdint>
#include <array>
#include <vector>

class Display {
public:
    static constexpr uint8_t WIDTH = 64;
    static constexpr uint8_t HEIGHT = 32;
    typedef std::array<std::array<bool, WIDTH>, HEIGHT> Pixels;

    bool draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite);
    const Pixels &get_pixels() const { return pixels; }
    void clear();
private:
    Pixels pixels {};
};

#endif //CHIP8_DISPLAY_H
