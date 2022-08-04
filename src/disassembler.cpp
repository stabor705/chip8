#include "disassembler.h"

#include <sstream>

const std::unordered_map<uint8_t, Disassembler::Template> Disassembler::f_mapping = {
        { 0x07, [](uint16_t instr){ return Disassembler::reg_instr("LD", instr) + ", DT"; } },
        { 0x0A, [](uint16_t instr){ return Disassembler::reg_instr("LD", instr) + ", K"; } },
        { 0x15, [](uint16_t instr){ return Disassembler::reg_instr("LD DT,", instr); } },
        { 0x18, [](uint16_t instr){ return Disassembler::reg_instr("LD ST,", instr); } },
        { 0x1E, [](uint16_t instr){ return Disassembler::reg_instr("ADD I,", instr); } },
        { 0x29, [](uint16_t instr){ return Disassembler::reg_instr("LD F,", instr); } },
        { 0x33, [](uint16_t instr){ return Disassembler::reg_instr("LD B,", instr); } },
        { 0x55, [](uint16_t instr){ return Disassembler::reg_instr("LD I,", instr); } },
        { 0x65, [](uint16_t instr){ return Disassembler::reg_instr("LD", instr) + ", I"; } }
};

std::string Disassembler::disassemble(uint16_t instr) {
    uint16_t idx = instr & 0xF000;
    idx = idx >> 12;
    return mapping[idx](instr);
}

std::string Disassembler::addr_instr(const char *name, uint16_t instr) {
    std::stringstream ss;
    ss << name << " #" << std::hex << int(instr & 0x0FFF);
    return ss.str();
}

std::string Disassembler::regs_instr(const char *name, uint16_t instr) {
    std::stringstream ss;
    uint16_t x = (instr & 0x0F00) >> 8;
    uint16_t y = (instr & 0x00F0) >> 4;
    ss << name << " V" << std::hex << int(x) << ", V" << int(y);
    return ss.str();
}

std::string Disassembler::reg_instr(const char *name, uint16_t instr) {
    std::stringstream ss;
    uint16_t x = (instr & 0x0F00) >> 8;
    ss << name << " V" << std::hex << int(x);
    return ss.str();
}

std::string Disassembler::reg_arg_instr(const char *name, uint16_t instr) {
    std::stringstream ss;
    uint16_t x = (instr & 0x0F00) >> 8;
    ss << name << " V" << std::hex << int(x) << ", #" << int(instr & 0x00FF);
    return ss.str();
}

std::string Disassembler::arithmetic_instr(uint16_t instr) {
    uint16_t idx = instr & 0x000F;
    if (idx < 8)
        return arithmetic_mapping[idx](instr);
    else if (idx == 0xE)
        return regs_instr("SHL", instr); // 0x8xxE instruction
    return {};
}

std::string Disassembler::zeroth_instr(uint16_t instr) {
    if (instr == 0x00E0)
        return "CLS";
    else if (instr == 0x00EE)
        return "RET";
    else
        return addr_instr("SYS", instr);
}

std::string Disassembler::draw_instr(uint16_t instr) {
    std::stringstream ss;
    uint16_t x = (instr & 0x0F00) >> 8;
    uint16_t y = (instr & 0x00F0) >> 4;
    uint16_t nibble = instr & 0x000F;
    ss << "DRW V" << std::hex << int(x) << ", V" << int(y) << ", " << nibble;
    return ss.str();
}

std::string Disassembler::e_instr(uint16_t instr) {
    if ((instr & 0x00FF) == 0x9E)
        return reg_instr("SKP", instr);
    else if ((instr & 0x00FF) == 0xA1)
        return reg_instr("SKNP", instr);
    else return {};
}

std::string Disassembler::f_instr(uint16_t instr) {
    auto it = f_mapping.find(instr & 0x00FF);
    if (it != f_mapping.end())
        return (it->second)(instr);
    return {};
}