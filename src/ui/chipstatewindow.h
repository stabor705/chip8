#ifndef CHIPSTATEWINDOW_H
#define CHIPSTATEWINDOW_H

#include "chip8/chip8.h"

class ChipStateWindow
{
public:
    void update(const Chip8 &chip);
    void show() const;
private:
    unsigned int v[16];
    unsigned int vi;
    unsigned int pc;
};

#endif // CHIPSTATEWINDOW_H
