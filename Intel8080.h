#ifndef INTEL8080_H
#define INTEL8080_H

#include <cstdint>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <functional>
#include <string>

class Intel8080;

typedef void (Intel8080::*func_t)(void);

struct Opcode
{
  const uint8_t number;
  const std::string mnemonic;
  const std::string fullName;
  func_t callback;

  Opcode(
    const uint8_t number,
    const std::string mnemonic,
    const std::string fullName,
    func_t callback
  );
};

class Intel8080
{
  public:
    Intel8080(void);
    void printMemory(void);
    void printMemory(int a, int b);
    void printRegisters(void);
    void loadProgram(std::vector<uint8_t> prog, uint16_t address);
    void execute(void);
    void setProgramCounter(uint16_t addr);

  private:
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;

    uint16_t sp;
    uint16_t pc;

    uint8_t flags;

    std::vector<uint8_t> memory;
    std::vector<Opcode> opcodes;

    enum class Register : uint8_t {
      A = 0b111,
      B = 0b000,
      C = 0b001,
      D = 0b010,
      E = 0b011,
      H = 0b100,
      L = 0b101,
      M = 0b110
    };

    bool terminateFlag = false;

    uint8_t getRegisterValue(Intel8080::Register reg);
    void setRegisterValue(Intel8080::Register reg, uint8_t val);
    void generateOpcodes(void);
    void executeInstruction(uint8_t opcode_n);

    void op_nop(void);

    void op_mvi(Intel8080::Register reg);
    void op_mvi_a(void);
    void op_mvi_b(void);
    void op_mvi_c(void);
    void op_mvi_d(void);
    void op_mvi_e(void);
    void op_mvi_h(void);
    void op_mvi_l(void);
    void op_mvi_m(void);

    void op_mov(Intel8080::Register destination, Intel8080::Register source);
    void op_mov_b_b(void);
    void op_mov_b_c(void);
    void op_mov_b_d(void);
    void op_mov_b_e(void);
    void op_mov_b_h(void);
    void op_mov_b_l(void);
    void op_mov_b_m(void);
    void op_mov_b_a(void);

    void op_mov_c_b(void);
    void op_mov_c_c(void);
    void op_mov_c_d(void);
    void op_mov_c_e(void);
    void op_mov_c_h(void);
    void op_mov_c_l(void);
    void op_mov_c_m(void);
    void op_mov_c_a(void);

    void op_mov_d_b(void);
    void op_mov_d_c(void);
    void op_mov_d_d(void);
    void op_mov_d_e(void);
    void op_mov_d_h(void);
    void op_mov_d_l(void);
    void op_mov_d_m(void);
    void op_mov_d_a(void);

    void op_mov_e_b(void);
    void op_mov_e_c(void);
    void op_mov_e_d(void);
    void op_mov_e_e(void);
    void op_mov_e_h(void);
    void op_mov_e_l(void);
    void op_mov_e_m(void);
    void op_mov_e_a(void);

    void op_mov_h_b(void);
    void op_mov_h_c(void);
    void op_mov_h_d(void);
    void op_mov_h_e(void);
    void op_mov_h_h(void);
    void op_mov_h_l(void);
    void op_mov_h_m(void);
    void op_mov_h_a(void);

    void op_mov_l_b(void);
    void op_mov_l_c(void);
    void op_mov_l_d(void);
    void op_mov_l_e(void);
    void op_mov_l_h(void);
    void op_mov_l_l(void);
    void op_mov_l_m(void);
    void op_mov_l_a(void);

    void op_mov_m_b(void);
    void op_mov_m_c(void);
    void op_mov_m_d(void);
    void op_mov_m_e(void);
    void op_mov_m_h(void);
    void op_mov_m_l(void);
    void op_mov_m_m(void);
    void op_mov_m_a(void);

    void op_mov_a_b(void);
    void op_mov_a_c(void);
    void op_mov_a_d(void);
    void op_mov_a_e(void);
    void op_mov_a_h(void);
    void op_mov_a_l(void);
    void op_mov_a_m(void);
    void op_mov_a_a(void);

    void op_dump(void);
    void op_term(void);
};


#endif
