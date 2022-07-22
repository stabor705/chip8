#ifndef CHIP8_SDLUTILS_H
#define CHIP8_SDLUTILS_H

#include <SDL2/SDL.h>

#include <exception>

class SDLError : public std::exception {
public:
    SDLError() = default;
    const char *what() const noexcept override {
        return SDL_GetError();
    }
};
#endif //CHIP8_SDLUTILS_H
