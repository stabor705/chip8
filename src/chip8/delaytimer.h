#ifndef DELAYTIMER_H
#define DELAYTIMER_H

#include <chrono>
namespace chrono = std::chrono;

/**
 * @brief A simulation of delay timer found in chip8 architecture.
 *
 * A very common need in game development is to count time remaining until some
 * event occurs. In chip8 architecture, this functionality is realized with a special
 * data register - delay timer. As it is the case with regular registers, you can store
 * an 8-bit value in it. The difference is that delay timer will perpetually count
 * down at a rate of 60 Hz untill it reaches 0.
 *
 * DelayTimer is responsible for delivering this functionality. You can store a value
 * by calling DelayTimer::store and later read it by calling DelayTimer::read.
 */
class DelayTimer {
public:
    DelayTimer() = default;
    void store(uint8_t value);

    /**
     * @brief Read value of delay timer
     *
     * Under the hood, instead of
     * decrementing registers' value by 1 every 1/60 second, it stores the value once,
     * and at each read returns the value stored minus
     * interval between last store timepoint and now, represented in 60 Hz ticks.
     *
     * @return Value of delay timer
     */
    uint8_t read();

private:
    using clock = chrono::steady_clock;
    using tick_duration = chrono::duration<int64_t, std::ratio<1, 60>>; // Delay timer ticks with 60Hz frequency

    clock::time_point store_tp;
    uint8_t value;
};

#endif // DELAYTIMER_H
