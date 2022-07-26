#include "chip8.h"

UndefinedInstruction::UndefinedInstruction(uint16_t instr) {
    std::stringstream ss;
    ss << "Encountered undefined instruction " << std::hex << (int)instr << '.';
    msg = ss.str();
}

LogicError::LogicError(uint16_t instr, const char *explanation) {
    std::stringstream ss;
    ss << "Instruction " << std::hex << int(instr) << " contains logical error: "
       << explanation;
    msg = ss.str();
}

ProgramSizeError::ProgramSizeError(size_t size) {
    std::stringstream ss;
    ss << "Program of size " << size << " bytes is too large";
    msg = ss.str();
}

constexpr std::array<Chip8::instr_impl, 16> Chip8::impls;
constexpr std::array<Chip8::instr_impl, 8> Chip8::arithmetic_ops;

Chip8::Chip8() : pc(PROGRAM_OFFSET), rng(chrono::steady_clock::now().time_since_epoch().count())
{
    load_hex_digits();
}

void Chip8::load_program(const std::vector<uint8_t> &program) {
    if (program.size() > memory.size() - PROGRAM_OFFSET)
        throw ProgramSizeError(program.size());
    this->program = program;
    load_program();
}

void Chip8::load_program() {
    std::copy(program.begin(), program.end(), memory.begin() + PROGRAM_OFFSET);
}

bool Chip8::run_program_instr() {
    if (pc >= PROGRAM_OFFSET + program.size())
        return false;
    uint16_t instr = memory[pc];
    instr = instr << 8;
    instr += memory[pc + 1];
    pc += 2;
    run_instr(instr);
    return true;
}

void Chip8::press_key(uint8_t key) {
    key_pressed = true;
    this->key = key;
}

void Chip8::reset() {
    std::fill(v.begin(), v.end(), 0);
    vi = 0;
    pc = PROGRAM_OFFSET;
    while(!call_stack.empty())
        call_stack.pop();
    key_pressed = false;
    dt.store(0);
    display.clear();
    std::fill(memory.begin(), memory.end(), 0);
    load_hex_digits();
    load_program();
}

bool Chip8::display_changed() {
    if (drawed_recently) {
        drawed_recently = false;
        return true;
    }
    return false;
}

void Chip8::run_instr(uint16_t instr) {
    uint16_t idx = (instr & 0xF000) >> 12;
    (this->*impls[idx])(instr);
}

void Chip8::subroutine_screen(uint16_t instr) {
    if (instr == 0x00E0) {
        display.clear();
        return;
    } else if (instr == 0x00EE) {
        if (call_stack.empty())
            throw LogicError(instr, "Call stack is empty");
        pc = call_stack.top();
        call_stack.pop();
        return;
    }
}

void Chip8::jump(uint16_t instr) {
    uint16_t addr = instr & 0x0FFF;
    pc = addr;
}

void Chip8::execute_subroutine(uint16_t instr) {
    uint16_t addr = instr & 0x0FFF;
    call_stack.push(pc);
    pc = addr;
}

void Chip8::skip_if_x_eq_arg(uint16_t instr) {
    uint8_t x = get_x_reg(instr);
    uint8_t arg = instr & 0x00FF;
    if (x == arg) pc += 2;
}

void Chip8::skip_if_x_ne_arg(uint16_t instr) {
    uint8_t x = get_x_reg(instr);
    uint8_t arg = instr & 0x00FF;
    if (x != arg) pc += 2;
}

void Chip8::skip_if_x_eq_y(uint16_t instr) {
    if ((instr & 0x000F) != 0)
        throw UndefinedInstruction(instr);
    uint8_t x = get_x_reg(instr);
    uint8_t y = get_y_reg(instr);
    if (x == y) pc += 2;
}

void Chip8::store_x(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint8_t value = instr & 0x0FF;
    v[x] = value;
}

void Chip8::add(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint8_t arg = instr & 0x00FF;
    v[x] += arg;
}

void Chip8::skip_if_x_ne_y(uint16_t instr) {
    if ((instr & 0x000F) != 0)
        throw UndefinedInstruction(instr);
    uint8_t x = get_x_reg(instr);
    uint8_t y = get_y_reg(instr);
    if (x != y) pc += 2;
}

void Chip8::store_i(uint16_t instr) {
    uint16_t arg = instr & 0x0FFF;
    vi = arg;
}

void Chip8::jump_using_v0(uint16_t instr) {
    uint16_t arg = instr & 0x0FFF;
    pc = v[0] + arg;
}

void Chip8::arithmetic(uint16_t instr) {
    uint16_t kind = instr & 0x000F;
    if (kind <= 7) {
        (this->*arithmetic_ops[kind])(instr);
    } else if (kind == 0xE) shift_y_left(instr);
    else throw UndefinedInstruction(instr);
}

void Chip8::random_number(uint16_t instr) {
    uint8_t byte_mask = instr & 0x00FF;
    uint16_t x = get_x_reg_idx(instr);
    uint8_t value = rng() % (0xFF + 1);
    v[x] = value & byte_mask;
}

void Chip8::draw(uint16_t instr) {
    uint8_t x = get_x_reg(instr);
    uint8_t y = get_y_reg(instr);
    uint8_t arg = instr & 0x000F;
    if (y >= Display::HEIGHT || x >= Display::WIDTH)
        throw LogicError(instr, "Program tries to draw out of bounds");
    std::vector<uint8_t> sprite(arg);
    for (int i = 0; i < arg; i++) {
        sprite[i] = memory[vi + i];
    }
    v[0xF] = display.draw_sprite(x, y, sprite);
    drawed_recently = true;
}

void Chip8::check_key(uint16_t instr) {
    uint8_t kind = instr & 0x00FF;
    uint8_t x = get_x_reg(instr);
    if (kind == 0x9E) {
        if (key_pressed && key == x)
            pc += 2;
        return;
    }
    else if (kind == 0xA1) {
        if (!key_pressed || key != x)
            pc += 2;
        return;
    }
    throw UndefinedInstruction(instr);
}

void Chip8::time_and_vi(uint16_t instr) {
    uint8_t kind = instr & 0x00FF;
    switch (kind) {
    case 0x07:
        store_dt(instr);
        break;
    case 0x0A:
        wait_for_keypress(instr);
        break;
    case 0x15:
        set_dt(instr);
        break;
    case 0x18:
        set_st(instr);
        break;
    case 0x1E:
        add_x_to_i(instr);
        break;
    case 0x29:
        set_i_to_hexdigit(instr);
        break;
    case 0x33:
        store_bcd(instr);
        break;
    case 0x55:
        store_registers(instr);
        break;
    case 0x65:
        fill_registers(instr);
        break;
    default:
        throw UndefinedInstruction(instr);
    }
}

void Chip8::store_y_in_x(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[x] = v[y];
}

void Chip8::x_or_y(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[x] = v[x] | v[y];
}

void Chip8::x_and_y(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[x] = v[x] & v[y];
}

void Chip8::x_xor_y(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[x] = v[x] ^ v[y];
}

void Chip8::add_y_to_x(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    uint16_t res = (uint16_t)(v[x]) + v[y];
    if (res > 255) v[0xF] = 1;
    else v[0xF] = 0;
    v[x] = res % (0xFF + 1);
}

void Chip8::sub_y_from_x(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    if (v[y] > v[x]) v[0xF] = 0;
    else v[0xF] = 1;
    v[x] = v[x] - v[y];
}

void Chip8::shift_y_right(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[0xF] = v[y] & 0b00000001;
    v[x] = v[y] >> 1;
}

void Chip8::y_minus_x(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    if (v[x] > v[y]) v[0xF] = 0;
    else v[0xF] = 1;
    v[x] = v[y] - v[x];
}

void Chip8::shift_y_left(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[0xF] = (v[y] & 0b10000000) >> 7;
    v[x] = v[y] << 1;
}

void Chip8::store_dt(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    v[x] = dt.read();
}

void Chip8::wait_for_keypress(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    if (key_pressed)
        v[x] = key;
    else
        pc -= 2;
}

void Chip8::set_dt(uint16_t instr) {
    dt.store(get_x_reg(instr));
}

void Chip8::set_st(uint16_t instr) {
    // TODO
}

void Chip8::add_x_to_i(uint16_t instr) {
    vi += get_x_reg(instr);
}

void Chip8::set_i_to_hexdigit(uint16_t instr) {
    vi = HEXDIGITS_OFFSET + get_x_reg(instr) * 5;
}

void Chip8::store_bcd(uint16_t instr) {
    uint8_t x = get_x_reg(instr);
    for (int i = 2; i >= 0; i--) {
        memory[vi + i] =  x % 10;
        x /= 10;
    }
}

void Chip8::store_registers(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    for (int i = 0; i <= x; i++) {
        memory[vi + i] =  v[i];
    }
    vi += x + 1;
}

void Chip8::fill_registers(uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    for (int i = 0; i <= x; i++) {
        v[i] = memory[vi + i];
    }
    vi += x + 1;
}

uint8_t Chip8::get_x_reg(uint16_t instr) {
    uint16_t reg_idx = get_x_reg_idx(instr);
    return v[reg_idx];
}

uint8_t Chip8::get_y_reg(uint16_t instr) {
    uint16_t reg_idx = get_y_reg_idx(instr);
    return v[reg_idx];
}

uint16_t Chip8::get_x_reg_idx(uint16_t instr) {
    return (instr & 0x0F00) >> 8;
}

uint16_t Chip8::get_y_reg_idx(uint16_t instr) {
    return (instr & 0x00F0) >> 4;
}

void Chip8::load_hex_digits() {
    auto addr = HEXDIGITS_OFFSET;
    for (const auto &hex_digit : hex_digits) {
        for (const auto byte : hex_digit) {
            memory[addr] =  byte;
            addr++;
        }
    }
}
