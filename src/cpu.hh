#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <memory>
#include <vector>

#include "ram.hh"

enum class Register : uint8_t {
  A, B, C, D, E, H, L, // main registers
  SP,                  // stack pointer treated as register
  M                    // Memory = [H:L]
};

enum class Flag : uint8_t {
  S = 0b10000000,  // Sign      - set if the result is negative
  Z = 0b01000000,  // Zero      - set if the result is zero
  Z1 = 0b00100000, // 0         - always 0
  AC = 0b00010000, // Aux carry - used for BCD arithmetic
  Z2 = 0b00001000, // 0         - always 0
  P = 0b00000100,  // Parity    - set if the no. of 1 bits is even
  O = 0b00000010,  // 1         - always 1
  C = 0b00000001,  // Carry     - set if last op resulted in a carry/borrow
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
    bool get_flag(Flag f);
    void set_flag(Flag f, bool set);
};

#endif
