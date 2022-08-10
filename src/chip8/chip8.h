#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include "chip8/mem.h"
#include "chip8/display.h"

#include "spdlog/spdlog.h"

#include <random>
#include <unordered_map>
#include <exception>
#include <sstream>
#include <stack>
#include <chrono>
#include <memory>
namespace chrono = std::chrono;

class UndefinedInstruction : public std::exception {
public:
    UndefinedInstruction(uint16_t instr) {
        std::stringstream ss;
        ss << "Encountered undefined instruction " << std::hex << instr << '.';
        msg = ss.str();
    }
    const char *what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};

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

class Chip8 {
public:
    Chip8();
    void load_program(const std::vector<uint8_t> &program);

    void run_instr(const uint16_t instr);
    uint16_t run_program_instr();

    uint16_t get_vi() const { return vi; }
    uint8_t get_v(uint16_t idx) const { return v[idx]; }
    uint16_t get_pc() const { return pc; }
    void set_vi(uint16_t  idx, uint8_t value) { v[idx] = value; }
    uint8_t get_vf() const { return v[0xF]; }
    const Memory& get_mem() const { return memory; }
    const Display::Pixels& get_screen() const { return display.get_pixels(); }
private:
    void subroutine_screen(uint16_t instr);
    void jump(uint16_t instr);
    void execute_subroutine(uint16_t instr);
    void skip_if_x_eq_arg(uint16_t instr);
    void skip_if_x_ne_arg(uint16_t instr);
    void skip_if_x_eq_y(uint16_t instr);
    void store_x(uint16_t instr);
    void add(uint16_t instr);
    void arithmetic(uint16_t instr);
    void skip_if_x_ne_y(uint16_t instr);
    void store_i(uint16_t instr);
    void jump_using_v0(uint16_t instr);
    void random_number(uint16_t instr);
    void draw(uint16_t instr);
    void check_key(uint16_t instr);
    void time_and_vi(uint16_t instr);

    void store_y_in_x(uint16_t instr);
    void x_or_y(uint16_t instr);
    void x_and_y(uint16_t instr);
    void x_xor_y(uint16_t instr);
    void add_y_to_x(uint16_t instr);
    void sub_y_from_x(uint16_t instr);
    void shift_y_right(uint16_t instr);
    void y_minus_x(uint16_t instr);
    void shift_y_left(uint16_t instr);

    void store_dt(uint16_t instr);
    void wait_for_keypress(uint16_t instr);
    void set_dt(uint16_t instr);
    void set_st(uint16_t instr);
    void add_x_to_i(uint16_t instr);
    void set_i_to_hexdigit(uint16_t instr);
    void store_bcd(uint16_t instr);
    void store_registers(uint16_t instr);
    void fill_registers(uint16_t instr);

    void load_hex_digits(uint16_t addr);

    static uint16_t get_x_reg_idx(uint16_t instr);
    uint8_t get_x_reg(uint16_t instr);
    static uint16_t get_y_reg_idx(uint16_t instr);
    uint8_t get_y_reg(uint16_t instr);
    typedef void(Chip8::*instr_impl)(uint16_t);

    static constexpr uint16_t PROGRAM_OFFSET = 0x200;
    static constexpr std::array<instr_impl, 16> impls = {
            &Chip8::subroutine_screen, &Chip8::jump, &Chip8::execute_subroutine,
            &Chip8::skip_if_x_eq_arg, &Chip8::skip_if_x_ne_arg, &Chip8::skip_if_x_eq_y,
            &Chip8::store_x, &Chip8::add, &Chip8::arithmetic,
            &Chip8::skip_if_x_ne_y, &Chip8::store_i, &Chip8::jump_using_v0,
            &Chip8::random_number, &Chip8::draw, &Chip8::check_key,
            &Chip8::time_and_vi
    };
    static constexpr std::array<instr_impl , 8> arithmetic_ops = {
        &Chip8::store_y_in_x, &Chip8::x_or_y, &Chip8::x_and_y,
        &Chip8::x_xor_y, &Chip8::add_y_to_x, &Chip8::sub_y_from_x,
        &Chip8::shift_y_right, &Chip8::y_minus_x,
    };
    static const std::unordered_map<uint8_t, instr_impl> f_ops;
    static constexpr std::array<std::array<uint8_t, 5>, 16> hex_digits = {{
            { 0xF0, 0x90, 0x90, 0x90, 0xF0 }, // 0
            { 0x20, 0x60, 0x20, 0x20, 0x70 }, // 1
            { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }, // 2
            { 0xF0, 0x10, 0xF0, 0x10, 0xF0 }, // 3
            { 0x90, 0x90, 0xF0, 0x10, 0x10 }, // 4
            { 0xF0, 0x50, 0xF0, 0x10, 0xF0 }, // 5
            { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }, // 6
            { 0xF0, 0x10, 0x20, 0x40, 0x40 }, // 7
            { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }, // 8
            { 0xF0, 0x90, 0xF0, 0x10, 0xF0 }, // 9
            { 0xF0, 0x90, 0xF0, 0x10, 0xF0 }, // A
            { 0xE0, 0x90, 0xE0, 0x90, 0xE0 }, // B
            { 0xF0, 0x80, 0x80, 0x80, 0xF0 }, // C
            { 0xE0, 0x90, 0x90, 0x90, 0xE0 }, // D
            { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }, // E
            { 0xF0, 0x80, 0xF0, 0x80, 0x80 }  // F
    }};

    std::array<uint8_t, 16> v; // Sixteen 8 bit registers
    uint16_t vi;  // 16 bit register that stores memory address
    uint16_t pc; // program counter
    std::stack<uint16_t> call_stack; // TODO: max call depth
    std::mt19937 rng; // TODO: Potential bottleneck. We only need to generate 8 bit pseudo-random numbers
    uint16_t hex_digits_addr;

    Memory memory;
    DelayTimer dt;
    Display display;
};

#endif //CHIP8_CHIP8_H
