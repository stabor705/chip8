#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include "chip8/display.h"
#include "chip8/delaytimer.h"

#include <random>
#include <exception>
#include <sstream>
#include <stack>

class UndefinedInstruction : public std::exception {
public:
    UndefinedInstruction(uint16_t instr);
    const char *what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};

class LogicError : public std::exception {
public:
    LogicError(uint16_t instr, const char *explanation);
    const char *what() const noexcept override {
        return msg.c_str();
    }
private:
    std::string msg;
};

class ProgramSizeError : public std::exception {
public:
    ProgramSizeError(size_t size);
    const char *what() const noexcept override {
        return msg.c_str();
    }
private:
    std::string msg;
};

using Program = std::vector<uint8_t>;
using Memory = std::array<uint8_t, 0xFFF>;

/**
 * @brief A chip8 emulator.
 *
 * It stores emulation state and provides an interface for running 2 byte chip8
 * instructions.
 * You can either run a chip8 program with it by calling Chip8::load_program
 * and then repeatedly calling Chip8::run_program_instr, or run single instructions
 * using Chip8::run_instr.
 *
 * While running the program, you can emulate pressing key on real console by
 * calling Chip8::press_key (there are 16 keys, each of them corresponding to 4 bit number
 * from 0x0 to 0xf) and then "see" produced graphics with Chip8::get_screen, which returns
 * current pixel array, a 2D array of booleans.
 *
 * Note that there are no public, helpful in unit tests, functions mapping
 * some expected functionality to 2 byte instruction (jump(addr) for instance).
 * You have to create the instruction code yourself and then call Chip8::run_instr.
 * This is intentional, as it greatly simplifies instruction parsing logic, which is
 * just a mapping between first byte of instruction and some private callback
 * (using std::array) without really causing much trouble.
 */
class Chip8 {
public:
    Chip8();
    /**
     * @brief Load a program into chips' memory.
     * @param program A byte vector storing raw instructions in big-endian order.
     */
    void load_program(const Program &program);

    /**
     * @brief Run 2 byte chip8 instruction.
     */
    void run_instr(const uint16_t instr);

    /**
     * @brief Run instruction currently stored in memory and pointed to by program
     * counter.
     * @return true on success, false if program counter ran past program memory space
     * and no instruction could be executed.
     */
    bool run_program_instr();

    /**
     * @brief Emulate pressing key on chip8 console.
     * @param key A integer in [0x0; 0xf] representing console key.
     * @note From this point on, chip will behave as if the key was pressed all the time.
     * You have to call Chip8::release_key in order to release it each time you call
     * Chip8::press_key.
     */
    void press_key(uint8_t key);
    void release_key() { key_pressed = false; }

    /**
     * @brief Reset emulation.
     *
     * This method cleans whole memory (program included), registers,
     * call stack and other state variables. After that, Chip8 can be used as if it
     * was just instantiated.
     */
    void reset();

    /**
     * @brief Get 64x32 binary display produced by Chip8.
     * @return A 64x32 array of booleans, each element representing one pixel.
     */
    const Display::Pixels& get_screen() const { return display.get_pixels(); }

    uint16_t get_vi() const { return vi; }
    uint8_t get_v(uint16_t idx) const { return v[idx]; }
    uint16_t get_pc() const { return pc; }
    void set_vi(uint16_t  idx, uint8_t value) { v[idx] = value; }
    uint8_t get_vf() const { return v[0xF]; }
    const Memory& get_mem() const { return memory; }
    uint8_t get_pressed_key() const { return (key_pressed) ? key : 0; }
    bool display_changed();
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

    void load_hex_digits();
    void load_program();

    static uint16_t get_x_reg_idx(uint16_t instr);
    uint8_t get_x_reg(uint16_t instr);
    static uint16_t get_y_reg_idx(uint16_t instr);
    uint8_t get_y_reg(uint16_t instr);
    typedef void(Chip8::*instr_impl)(uint16_t);

    static constexpr uint16_t PROGRAM_OFFSET = 0x200;
    static constexpr uint16_t HEXDIGITS_OFFSET = 0x0;
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

    std::array<uint8_t, 16> v {}; // Sixteen 8 bit registers
    uint16_t vi {};  // 16 bit register that stores memory address
    uint16_t pc; // program counter
    std::stack<uint16_t> call_stack;
    uint8_t key;
    bool key_pressed {};
    bool drawed_recently {};

    std::mt19937 rng;
    Memory memory;
    DelayTimer dt;
    Display display;

    Program program;
};

#endif //CHIP8_CHIP8_H
