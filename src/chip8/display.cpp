#include "display.h"

bool Display::draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite) {
    bool set_vf = false;
    for (int row = 0; row < sprite.size(); row++) {
        int bit = 1;
        for (int col = 0; col < 8; col++) {
            bool pixel = pixels[row + y][col + x];
            bool sprite_pixel = sprite[row] & bit;
            if (pixel && sprite_pixel)
                set_vf = true;
            pixels[row + y][col + x] = pixel ^ sprite_pixel;
        }
    }
    return set_vf;
}

void Display::clear() {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            pixels[y][x] = false;
        }
    }
}
