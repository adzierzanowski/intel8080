#ifndef INTEL8080_H
#define INTEL8080_H

#include <cstdint>
#include <vector>
#include <cstdio>
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

    enum class Flag : uint8_t {
      C = 0,
      P = 2,
      AC = 4,
      Z = 6,
      S = 7
    };

    enum class Condition : uint8_t {
      ZERO_FLAG_NOT_SET = 0b000,
      ZERO_FLAG_SET = 0b001,
      CARRY_FLAG_NOT_SET = 0b010,
      CARRY_FLAG_SET = 0b011,
      PARITY_FLAG_NOT_SET = 0b100,
      PARITY_FLAG_SET = 0b101,
      SIGN_FLAG_NOT_SET = 0b110,
      SIGN_FLAG_SET = 0b111
    };

    Opcode *currentOpcode = nullptr;
    bool terminateFlag = false;

    uint8_t getRegisterValue(Intel8080::Register reg);
    void setRegisterValue(Intel8080::Register reg, uint8_t val);
    void generateOpcodes(void);
    void executeInstruction(Opcode opcode);

    bool getFlag(Intel8080::Flag flag);
    void setFlag(Intel8080::Flag flag);
    void resetFlag(Intel8080::Flag flag);

    bool getFlag(int pos);
    void setFlag(int pos, int state);
    void setFlags(uint8_t which, uint8_t old_val, uint8_t new_val);


    bool checkParity(uint8_t val);
    bool checkForAuxiliaryCarry(uint8_t old_val, uint8_t new_val);

    void op_nop(void);

    template <Intel8080::Register reg>
    void op_inr(void);

    template<Intel8080::Register reg>
    void op_dcr(void);

    template <Intel8080::Register reg>
    void op_mvi(void);

    template <Intel8080::Register destination, Intel8080::Register source>
    void op_mov(void);

    void op_jmp(void);
    
    template <Intel8080::Condition condition>
    void op_j(void);

    void op_xchg(void);

    void op_dump(void);
    void op_term(void);
};

#endif
