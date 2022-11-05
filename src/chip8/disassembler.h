#ifndef CHIP8_DISASSEMBLER_H
#define CHIP8_DISASSEMBLER_H

#include <cstdint>
#include <string>
#include <array>
#include <unordered_map>

/**
 * @brief A disassembler turning 2 bytes chip8 instructions into human readable strings.
 *
 * This class stores all functionality required to turn a 2 byte chip8 instruction
 * into human readable string. It has only one public method - Disassembler::disassemble -
 * which returns disassembly of provided instruction.
 *
 * The returned strings conform to
 * [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
 */
class Disassembler {
public:
    /**
     * @brief Disassemble chip8 instruction
     * @param instr A 2 byte chip8 instruction.
     * @return A human readable string representing given instruction.
     */
    std::string disassemble(uint16_t instr);
private:
    static std::string addr_instr(const char* name, uint16_t instr);
    static std::string regs_instr(const char* name, uint16_t instr);
    static std::string reg_arg_instr(const char *name, uint16_t instr);
    static std::string reg_instr(const char *name, uint16_t instr);
    static std::string arithmetic_instr(uint16_t instr);
    static std::string zeroth_instr(uint16_t instr);
    static std::string draw_instr(uint16_t instr);
    static std::string e_instr(uint16_t instr);
    static std::string f_instr(uint16_t instr);

    typedef std::string(*Template)(uint16_t);
    static constexpr std::array<Template, 16> mapping = {{
        zeroth_instr,
        [](uint16_t instr){ return addr_instr("JP", instr); },
        [](uint16_t instr){ return addr_instr("CALL", instr); },
        [](uint16_t instr){ return reg_arg_instr("SE", instr); },
        [](uint16_t instr){ return reg_arg_instr("SNE", instr); },
        [](uint16_t instr){ return regs_instr("SE", instr); },
        [](uint16_t instr){ return reg_arg_instr("LD", instr); },
        [](uint16_t instr){ return reg_arg_instr("ADD", instr); },
        arithmetic_instr,
        [](uint16_t instr){ return regs_instr("SNE", instr); },
        [](uint16_t instr){ return addr_instr("LD I,", instr); },
        [](uint16_t instr){ return addr_instr("JP V0,", instr); },
        [](uint16_t instr){ return reg_arg_instr("RND", instr); },
        draw_instr,
        e_instr,
        f_instr
    }};

    static constexpr std::array<Template, 8> arithmetic_mapping = {{
        [](uint16_t instr){ return regs_instr("LD", instr); },
        [](uint16_t instr){ return regs_instr("OR", instr); },
        [](uint16_t instr){ return regs_instr("AND", instr); },
        [](uint16_t instr){ return regs_instr("XOR", instr); },
        [](uint16_t instr){ return regs_instr("ADD", instr); },
        [](uint16_t instr){ return regs_instr("SUB", instr); },
        [](uint16_t instr){ return regs_instr("SHR", instr); },
        [](uint16_t instr){ return regs_instr("SUBN", instr); }
    }};

    static const std::unordered_map<uint8_t, Template> f_mapping;
};


#endif //CHIP8_DISASSEMBLER_H
