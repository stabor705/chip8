#include "keyboard.h"

#include <string>

Keyboard::Keyboard(const Keyboard::KeynamesMapping &mapping) {
    for (int i = 0; i < this->mapping.size(); i++) {
        this->mapping[i] = SDL_GetScancodeFromName(mapping[i].c_str());
    }
}

bool Keyboard::is_pressed(uint8_t key) const {
    return SDL_GetKeyboardState(NULL)[mapping[key]];
}

uint8_t Keyboard::wait_for_key() const {
    SDL_Event event;
    while (true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_KEYDOWN) {
            SDL_Scancode scancode = event.key.keysym.scancode;
            for (int i = 0; i < 15; i++) {
                if (mapping[i] == scancode)
                    return i;
            }
        }
    }
}
