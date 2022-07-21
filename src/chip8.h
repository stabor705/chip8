#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include "mem.h"

#include <functional>
#include <random>
#include <unordered_map>
#include <exception>
#include <sstream>
#include <stack>

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

class Chip8 {
public:
    Chip8();

    void load_program(std::istream &is);
    void run();

    void run_instr(const uint16_t instr);

    uint16_t get_vi() const { return vi; }
    uint8_t get_v(uint16_t idx) const { return v[idx]; }
    uint16_t get_pc() const { return pc; }
    void set_vi(uint16_t  idx, uint8_t value) { v[idx] = value; }
    uint8_t get_vf() const { return v[0xF]; }
    const Memory& get_mem() const { return memory; }
private:
    void subroutine_screen(const uint16_t instr);
    void jump(const uint16_t instr);
    void execute_subroutine(const uint16_t instr);
    void skip_if_x_eq_arg(const uint16_t instr);
    void skip_if_x_ne_arg(const uint16_t instr);
    void skip_if_x_eq_y(const uint16_t instr);
    void store_x(const uint16_t instr);
    void add(const uint16_t instr);
    void arithmetic(const uint16_t instr);
    void skip_if_x_ne_y(const uint16_t instr);
    void store_i(const uint16_t instr);
    void jump_using_reg(const uint16_t instr);
    void random_number(const uint16_t instr);
    void draw(const uint16_t instr);
    void skip_if_key_pressed(const uint16_t instr);
    void time_and_vi(const uint16_t instr);

    void store_y_in_x(const uint16_t instr);
    void x_or_y(const uint16_t instr);
    void x_and_y(const uint16_t instr);
    void x_xor_y(const uint16_t instr);
    void add_y_to_x(const uint16_t instr);
    void sub_y_from_x(const uint16_t instr);
    void shift_y_right(const uint16_t instr);
    void y_minus_x(const uint16_t instr);
    void shift_y_left(const uint16_t instr);

    void store_dt(const uint16_t instr);
    void wait_for_keypress(const uint16_t instr);
    void set_dt(const uint16_t instr);
    void set_st(const uint16_t instr);
    void add_x_to_i(const uint16_t instr);
    void set_i_to_hexdigit(const uint16_t instr);
    void store_bcd(const uint16_t instr);
    void store_registers(const uint16_t instr);
    void fill_registers(const uint16_t instr);

    static uint16_t get_x_reg_idx(const uint16_t instr);
    uint8_t get_x_reg(const uint16_t instr);
    static uint16_t get_y_reg_idx(const uint16_t instr);
    uint8_t get_y_reg(const uint16_t instr);
    typedef void(Chip8::*instr_impl)(const uint16_t);

    static constexpr uint16_t PROGRAM_OFFSET = 0x200;
    static constexpr std::array<instr_impl, 16> impls = {
            &Chip8::subroutine_screen, &Chip8::jump, &Chip8::execute_subroutine,
            &Chip8::skip_if_x_eq_arg, &Chip8::skip_if_x_ne_arg, &Chip8::skip_if_x_eq_y,
            &Chip8::store_x, &Chip8::add, &Chip8::arithmetic,
            &Chip8::skip_if_x_ne_y, &Chip8::store_i, &Chip8::jump_using_reg,
            &Chip8::random_number, &Chip8::draw, &Chip8::skip_if_key_pressed,
            &Chip8::time_and_vi
    };
    static constexpr std::array<instr_impl , 8> arithmetic_ops = {
        &Chip8::store_y_in_x, &Chip8::x_or_y, &Chip8::x_and_y,
        &Chip8::x_xor_y, &Chip8::add_y_to_x, &Chip8::sub_y_from_x,
        &Chip8::shift_y_right, &Chip8::y_minus_x,
    };
    static const std::unordered_map<uint8_t, instr_impl> f_ops;

    std::mt19937 rng; // TODO: Potential bottleneck. We only need to generate 8 bit pseudo-random numbers
    std::array<uint8_t, 16> v; // Sixteen 8 bit registers
    uint16_t vi;  // 16 bit register that stores memory address
    uint16_t pc; // program counter
    std::stack<uint16_t> call_stack; // TODO: max call depth

    Memory memory;
};


#endif //CHIP8_CHIP8_H