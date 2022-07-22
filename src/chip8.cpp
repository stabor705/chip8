#include "chip8.h"

void DelayTimer::store(uint8_t value) {
    store_tp = clock::now();
    this->value = value;
}

uint8_t DelayTimer::read() {
    auto ticks = chrono::floor<tick_duration>(clock::now() - store_tp).count();
    if (ticks >= value) return 0;
    else return value - ticks;
}

const std::unordered_map<uint8_t, Chip8::instr_impl> Chip8::f_ops = {
        { 0x07, &Chip8::store_dt }, { 0x0A, &Chip8::wait_for_keypress },
        { 0x15, &Chip8::set_dt }, { 0x18, &Chip8::set_st },
        { 0x1E, &Chip8::add_x_to_i }, { 0x29, &Chip8::set_i_to_hexdigit },
        { 0x33, &Chip8::store_bcd }, { 0x55, &Chip8::store_registers },
        { 0x65, &Chip8::fill_registers }
};
constexpr std::array<Chip8::instr_impl, 16> Chip8::impls;
constexpr std::array<Chip8::instr_impl, 8> Chip8::arithmetic_ops;

Chip8::Chip8() : pc(PROGRAM_OFFSET), rng(chrono::steady_clock::now().time_since_epoch().count()),
                 keyboard({"Q", "W", "E", "R", "T", "A", "S", "D", "F", "G", "Z", "X", "C", "V", "B", "Space"}),
                 display(new SDLDisplay(16))
{}

uint16_t Chip8::run_program_instr() {
    uint16_t instr = memory.fetch_instruction(pc);
    spdlog::debug("Running instruction {0:x}", instr);
    run_instr(instr);
    return instr;
}

void Chip8::run_instr(const uint16_t instr) {
    uint16_t idx = (instr & 0xF000) >> 12;
    (this->*impls[idx])(instr);
}

void Chip8::subroutine_screen(const uint16_t instr) {
    pc += 2;
    if (instr == 0x00E0) {
        display->clear_screen();
    } else if (instr == 0x00EE) {
        pc = call_stack.top();
        call_stack.pop();
        return;
    }
    spdlog::warn("Skipping instruction {0:x}", instr);
    // Ignore machine code related instruction
}

void Chip8::jump(const uint16_t instr) {
    uint16_t addr = instr & 0x0FFF;
    pc = addr;
}

void Chip8::execute_subroutine(const uint16_t instr) {
    pc += 2;
    uint16_t addr = instr & 0x0FFF;
    call_stack.push(pc);
    pc = addr;
}

void Chip8::skip_if_x_eq_arg(const uint16_t instr) {
    pc += 2;
    uint8_t x = get_x_reg(instr);
    uint8_t arg = instr & 0x00FF;
    if (x == arg) pc += 2;
}

void Chip8::skip_if_x_ne_arg(const uint16_t instr) {
    pc += 2;
    uint8_t x = get_x_reg(instr);
    uint8_t arg = instr & 0x00FF;
    if (x != arg) pc += 2;
}

void Chip8::skip_if_x_eq_y(const uint16_t instr) {
    if ((instr & 0x000F) != 0)
        throw UndefinedInstruction(instr);
    pc += 2;
    uint8_t x = get_x_reg(instr);
    uint8_t y = get_y_reg(instr);
    if (x == y) pc += 2;
}

void Chip8::store_x(const uint16_t instr) {
    pc += 2;
    uint16_t x = get_x_reg_idx(instr);
    uint8_t value = instr & 0x0FF;
    v[x] = value;
}

void Chip8::add(const uint16_t instr) {
    pc += 2;
    uint16_t x = get_x_reg_idx(instr);
    uint8_t arg = instr & 0x00FF;
    v[x] += arg;
}

void Chip8::skip_if_x_ne_y(const uint16_t instr) {
    if ((instr & 0x000F) != 0)
        throw UndefinedInstruction(instr);
    pc += 2;
    uint8_t x = get_x_reg(instr);
    uint8_t y = get_y_reg(instr);
    if (x != y) pc += 2;
}

void Chip8::store_i(const uint16_t instr) {
    pc += 2;
    uint16_t arg = instr & 0x0FFF;
    vi = arg;
}

void Chip8::jump_using_reg(const uint16_t instr) {
    uint16_t arg = instr & 0x0FFF;
    pc = v[0] + arg;
}

void Chip8::arithmetic(const uint16_t instr) {
    pc += 2;
    uint16_t kind = instr & 0x000F;
    if (kind <= 7) {
        (this->*arithmetic_ops[kind])(instr);
    } else if (kind == 0xE) shift_y_left(instr);
    else throw UndefinedInstruction(instr);
}

void Chip8::random_number(const uint16_t instr) {
    pc += 2;
    uint8_t byte_mask = instr & 0x00FF;
    uint16_t x = get_x_reg_idx(instr);
    uint8_t value = rng() % (0xFF + 1);
    v[x] = value & byte_mask;
}

void Chip8::draw(const uint16_t instr) {
    pc += 2;
    uint8_t x = get_x_reg(instr);
    uint8_t y = get_y_reg(instr);
    uint8_t arg = instr & 0x000F;
    std::vector<uint8_t> sprite(arg);
    for (int i = 0; i < arg; i++) {
        sprite[i] = memory.get(vi + i);
    }
    v[0xF] = display->draw_sprite(x, y, sprite);
}

void Chip8::skip_if_key_pressed(const uint16_t instr) {
    pc += 2;
    spdlog::warn("Skipping instruction {0:x}", instr);
    // TODO:
}

void Chip8::time_and_vi(const uint16_t instr) {
    pc += 2;
    uint8_t kind = instr & 0x00FF;
    (this->*f_ops.at(kind))(instr);
}

void Chip8::store_y_in_x(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[x] = v[y];
}

void Chip8::x_or_y(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[x] = v[x] | v[y];
}

void Chip8::x_and_y(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[x] = v[x] & v[y];
}

void Chip8::x_xor_y(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[x] = v[x] ^ v[y];
}

void Chip8::add_y_to_x(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    uint16_t res = (uint16_t)(v[x]) + v[y];
    if (res > 255) v[0xF] = 1;
    else v[0xF] = 0;
    v[x] = res % (0xFF + 1);
}

void Chip8::sub_y_from_x(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    if (v[y] > v[x]) v[0xF] = 0;
    else v[0xF] = 1;
    v[x] = v[x] - v[y];
}

void Chip8::shift_y_right(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[0xF] = v[y] & 0b00000001;
    v[x] = v[y] >> 1;
}

void Chip8::y_minus_x(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    if (v[x] > v[y]) v[0xF] = 0;
    else v[0xF] = 1;
    v[x] = v[y] - v[x];
}

void Chip8::shift_y_left(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    uint16_t y = get_y_reg_idx(instr);
    v[0xF] = (v[y] & 0b10000000) >> 7;
    v[x] = v[y] << 1;
}

void Chip8::store_dt(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    v[x] = dt.read();
}

void Chip8::wait_for_keypress(const uint16_t instr) {
    spdlog::warn("Skipping instruction {0:x}", instr);
    // TODO
}

void Chip8::set_dt(const uint16_t instr) {
    dt.store(get_x_reg(instr));
}

void Chip8::set_st(const uint16_t instr) {
    spdlog::warn("Skipping instruction {0:x}", instr);
    // TODO
}

void Chip8::add_x_to_i(const uint16_t instr) {
    vi += get_x_reg(instr);
}

void Chip8::set_i_to_hexdigit(const uint16_t instr) {
    spdlog::warn("Skipping instruction {0:x}", instr);
    // TODO
}

void Chip8::store_bcd(const uint16_t instr) {
    uint8_t x = get_x_reg(instr);
    for (int i = 2; i >= 0; i--) {
        memory.set(vi + i, x % 10);
        x /= 10;
    }
}

void Chip8::store_registers(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    for (int i = 0; i <= x; i++) {
        memory.set(vi + i, v[i]);
    }
    vi += x + 1;
}

void Chip8::fill_registers(const uint16_t instr) {
    uint16_t x = get_x_reg_idx(instr);
    for (int i = 0; i <= x; i++) {
        v[i] = memory.get(vi + i);
    }
    vi += x + 1;
}

uint8_t Chip8::get_x_reg(const uint16_t instr) {
    uint16_t reg_idx = get_x_reg_idx(instr);
    return v[reg_idx];
}

uint8_t Chip8::get_y_reg(const uint16_t instr) {
    uint16_t reg_idx = get_y_reg_idx(instr);
    return v[reg_idx];
}

uint16_t Chip8::get_x_reg_idx(const uint16_t instr) {
    return (instr & 0x0F00) >> 8;
}

uint16_t Chip8::get_y_reg_idx(const uint16_t instr) {
    return (instr & 0x00F0) >> 4;
}

void Chip8::load_program(const std::vector<uint8_t> &program) {
    // TODO: maybe check program length?
    for (int i = 0; i < program.size(); i++) {
        memory.set(PROGRAM_OFFSET + i, program[i]);
    }
}