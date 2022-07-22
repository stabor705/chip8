#include "display.h"

#include <spdlog/spdlog.h>


bool PixelMatrix::draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite) {
    bool set_vf = false;
    for (int row = 0; row < sprite.size(); row++) {
        int bit = 1;
        for (int col = 0; col < 8; col++) {
            bool pixel = matrix[row + y][col + x];
            bool sprite_pixel = sprite[row] & bit;
            if (pixel && sprite_pixel)
                set_vf = true;
            matrix[row + y][col + x] = pixel ^ sprite_pixel;
        }
    }
    return set_vf;
}

void PixelMatrix::clear() {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            matrix[y][x] = false;
        }
    }
}

SDLDisplay::SDLDisplay(uint8_t scale) : scale(scale) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        spdlog::error("Failed to initialize SDL Video subsystem");
        return;
    }
    SDL_CreateWindowAndRenderer(scale * 64, scale * 32, NULL, &window, &renderer);
}

bool SDLDisplay::draw_sprite(uint8_t x, uint8_t y, const std::vector<uint8_t> &sprite) {
    bool vf = pixel_matrix.draw_sprite(x, y, sprite);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_Rect rect;
    rect.y = 0;
    rect.w = rect.h = scale;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int row = 0; row < 32; row++) {
        rect.x = 0;
        for (int col = 0; col < 64; col++) {
            SDL_RenderDrawRect(renderer, &rect);
            rect.x += scale;
        }
        rect.y += scale;
    }
    SDL_RenderPresent(renderer);
    return vf;
}

void SDLDisplay::clear_screen() {
    pixel_matrix.clear();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

SDLDisplay::~SDLDisplay() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}