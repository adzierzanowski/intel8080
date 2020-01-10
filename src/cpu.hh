#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <memory>
#include <vector>

#include "ram.hh"

enum class Register : uint8_t {
  A, B, C, D, E, H, L, SP, M
};

class CPU
{
  public:
    // Registers
    uint8_t a, b, c, d, e, h, l;
    uint16_t sp; // Stack pointer
    uint16_t pc; // Program counter
    uint8_t flags; // Flags register

    std::unique_ptr<RAM> ram;

  public:
    CPU(void);
    void load_program(std::vector<uint8_t> prog, uint16_t addr);
    size_t get_memory_size(void);
    uint8_t get_opcode(void);
    void increase_program_counter(uint16_t by=1);
    uint8_t get_register(Register x);
    void set_register(Register x, uint8_t value);
    uint16_t get_register_pair(Register x, Register y);
    void set_register_pair(Register x, Register y, uint16_t value);
    uint8_t get_imm8(void);
    uint16_t get_imm16(void);
    void store(uint16_t addr, uint8_t value);
    uint8_t load(uint16_t addr);
    uint16_t get_pc(void);
    uint16_t get_stack_pointer(void);
    void set_stack_pointer(uint16_t value);
    void increase_stack_pointer(uint16_t by=1);
    void decrease_stack_pointer(uint16_t by=1);
};

#endif
