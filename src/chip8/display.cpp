#include "display.h"

bool Display::draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite) {
    bool collided = false;
    for (int row = 0; row < sprite.size(); row++) {
        for (int col = 0; col < 8; col++) {
            bool pixel = pixels[row + y][col + x];
            bool sprite_pixel = sprite[row] & (1 << col);
            if (pixel && sprite_pixel)
                collided = true;
            pixels[row + y][col + x] = pixel ^ sprite_pixel;
        }
    }
    return collided;
}

void Display::clear() {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            pixels[y][x] = false;
        }
    }
}
