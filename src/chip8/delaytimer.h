#ifndef DELAYTIMER_H
#define DELAYTIMER_H

#include <chrono>
namespace chrono = std::chrono;

class DelayTimer {
public:
    DelayTimer() = default;
    void store(uint8_t value);
    uint8_t read();

private:
    using clock = chrono::steady_clock;
    using tick_duration = chrono::duration<int64_t, std::ratio<1, 60>>; // Delay timer ticks with 60Hz frequency

    clock::time_point store_tp;
    uint8_t value;
};

#endif // DELAYTIMER_H
