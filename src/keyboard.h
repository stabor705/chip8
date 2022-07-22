#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include "sdlutils.h"

#include <memory>
#include <cstdint>
#include <array>

class Keyboard {
public:
    typedef std::array<std::string, 16> KeynamesMapping;
    typedef std::array<SDL_Scancode, 16> Mapping;

    Keyboard(const KeynamesMapping &mapping);

    bool is_pressed(uint8_t key) const;
    uint8_t wait_for_key() const;
private:
    Mapping mapping;
};


#endif //CHIP8_KEYBOARD_H
