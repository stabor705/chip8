#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include "sdlutils.h"

#include <cstdint>
#include <array>
#include <vector>

class PixelMatrix {
public:
    PixelMatrix() = default;

    bool draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite);
    bool get_pixel(uint8_t x, uint8_t y) const { return matrix[y][x]; }
    void clear();

private:
    std::array<std::array<bool , 64>, 32> matrix;
};

class Display {
public:
    virtual bool draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite) = 0;
    virtual void clear_screen() = 0;
    virtual ~Display() = default;

protected:
    PixelMatrix pixel_matrix;
};

class SDLDisplay : public Display {
public:
    SDLDisplay(uint8_t scale);

    virtual bool draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite) override;
    void clear_screen() override;

    ~SDLDisplay() override;

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    uint8_t scale;
};

#endif //CHIP8_DISPLAY_H