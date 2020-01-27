#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <memory>
#include <vector>

#include "ram.hh"

enum class Register : uint8_t {
  A, B, C, D, E, H, L, // main registers
  SP,                  // stack pointer treated as register
  M,                   // Memory = [H:L]
  FLAGS                // FLAGS
};

enum class Flag : uint8_t {
  S = 0b10000000,  // Sign      - set if the result is negative
  Z = 0b01000000,  // Zero      - set if the result is zero
  Z1 = 0b00100000, // 0         - always 0
  AC = 0b00010000, // Aux carry - used for BCD arithmetic
  Z2 = 0b00001000, // 0         - always 0
  P = 0b00000100,  // Parity    - set if the no. of 1 bits is even
  O = 0b00000010,  // 1         - always 1
  C = 0b00000001  // Carry     - set if last op resulted in a carry/borrow
};

class CPU
{
  friend class Emulator;

  private:
    // Registers
    uint8_t a, b, c, d, e, h, l;
    uint16_t sp; // Stack pointer
    uint16_t pc; // Program counter
    uint8_t flags; // Flags register

    std::unique_ptr<RAM> ram;

  public:
    static bool aux_carry(uint8_t before, uint8_t after);
    static bool even_parity(uint8_t val);

    CPU(void);
    void load_program(std::vector<uint8_t> prog, uint16_t addr);
    size_t get_memory_size(void);
    uint8_t get_opcode(void);

    uint8_t get_register(Register x);
    void set_register(Register x, uint8_t value);
    uint16_t get_register_pair(Register x, Register y);
    void set_register_pair(Register x, Register y, uint16_t value);

    uint8_t get_imm8(void);
    uint16_t get_imm16(void);
    void store(uint16_t addr, uint8_t value);
    uint8_t load(uint16_t addr);

    uint16_t get_pc(void);
    void set_pc(uint16_t addr);
    void increment_pc(uint16_t by=1);

    uint16_t get_sp(void);
    void set_sp(uint16_t value);
    void increment_sp(uint16_t by=1);
    void decrement_sp(uint16_t by=1);

    bool get_flag(Flag f);
    void set_flag(Flag f, bool set);
    void affect_flags_szap(uint8_t before, uint8_t after);

    void push(uint16_t addr);
    uint16_t pop(void);
};

#endif
