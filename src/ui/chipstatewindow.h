#ifndef CHIPSTATEWINDOW_H
#define CHIPSTATEWINDOW_H

#include "chip8/chip8.h"

class ChipStateWindow
{
public:
    void update(const Chip8 &chip);
    void show(float width, float height) const;
private:
    unsigned int v[16];
    unsigned int vi;
    unsigned int pc;
    unsigned int key;
};

#endif // CHIPSTATEWINDOW_H
