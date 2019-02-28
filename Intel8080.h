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
  const uint8_t length;
  const std::string mnemonic;
  const std::string fullName;
  func_t callback;

  Opcode(
    const uint8_t number,
    const uint8_t length,
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

    Opcode *currentOpcode = nullptr;
    bool terminateFlag = false;

    uint8_t getRegisterValue(Intel8080::Register reg);
    void setRegisterValue(Intel8080::Register reg, uint8_t val);
    void generateOpcodes(void);
    void executeInstruction(Opcode opcode);

    void setFlag(int pos, int state);
    void setCarryFlag(void);
    void resetCarryFlag(void);
    void setAuxiliaryCarryFlag(void);
    void resetAuxiliaryCarryFlag(void);
    void setSignFlag(void);
    void resetSignFlag(void);
    void setZeroFlag(void);
    void resetZeroFlag(void);
    void setParityFlag(void);
    void resetParityFlag(void);

    void op_nop(void);

    template <Intel8080::Register reg>
    void op_inr(void);

    template<Intel8080::Register reg>
    void op_dcr(void);

    template <Intel8080::Register reg>
    void op_mvi(void);

    template <Intel8080::Register destination, Intel8080::Register source>
    void op_mov(void);

    void op_dump(void);
    void op_term(void);
};

#endif
