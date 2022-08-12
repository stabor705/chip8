#include "delaytimer.h"

void DelayTimer::store(uint8_t value) {
    store_tp = clock::now();
    this->value = value;
}

uint8_t DelayTimer::read() {
    auto ticks = chrono::floor<tick_duration>(clock::now() - store_tp).count();
    if (ticks >= value) return 0;
    else return value - ticks;
}
