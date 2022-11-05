#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <cstdint>
#include <array>
#include <vector>

/**
 * @brief chip8 display interface
 *
 * In chip8 architecture, display is a 64x32 matrix, with each pixel representing
 * one of 2 color values, that is to say, it is a matrix of 1s and 0s. This class
 * stores these binary values as a 2D array of booleans. Additionaly, its interface
 * does not work by drawing in pixel-by-pixel manner. Instead, the user is provided
 * with method for properly drawing whole sprite at once - Display::draw_sprite.
 *
 * By default, all pixels are set to 0.
 */
class Display {
public:
    static constexpr uint8_t WIDTH = 64;
    static constexpr uint8_t HEIGHT = 32;
    typedef std::array<std::array<bool, WIDTH>, HEIGHT> Pixels;

    /**
     * @brief Draw sprite at given position in a manner that conforms to how real chip8
     * works.
     *
     * Drawing in chip8 architecture is a little bit more involved than you would expect.
     * Basically, the sprite, always 8 bits wide and with variable height, is drawn
     * starting from (X, Y) point. However, there are a couple of peculiarities to keep in
     * mind.
     *
     * 1. If sprite is drawn on previously set pixel, the display has to signal, that
     * collision has occured.
     * 2. Sprite is not really drawn by setting displays' pixels to corresponding values
     * of sprites' pixels. Instead, display is XORed with sprite data.
     * 3. The value of X can't be greater than 0x3F and value of Y can't be grater than
     * 0xF1.
     *
     * @param x X coordinate, read from left to right
     * @param y Y coordinate, read from top to bottom
     * @param sprite 8xN sprite data given as a vector of bytes, each byte representing
     * one row of 1s and 0s, read from top to bottom
     * @return True if sprite pixel collided with previously set pixel, false otherwise.
     */
    bool draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite);
    const Pixels &get_pixels() const { return pixels; }
    void clear();
private:
    Pixels pixels {};
};

#endif //CHIP8_DISPLAY_H
