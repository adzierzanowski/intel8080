#ifndef INTEL8080_H
#define INTEL8080_H

#include <cstdint>
#include <vector>
#include <cstdio>
#include <string>
#include <utility>

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
    bool debugOutput = true;

    Intel8080(void);
    void printMemory(void);
    void printMemory(int a, int b);
    void printRegisters(void);
    void loadProgram(std::vector<uint8_t> prog, uint16_t address);
    void execute(void);
    void setProgramCounter(uint16_t addr);
    std::vector<Opcode> opcodes;

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

    enum class RegisterPair : uint8_t {
      BC = 0b00,
      DE = 0b01,
      HL = 0b10,
      SP = 0b11
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
    uint16_t getRegisterPairValue(Intel8080::Register reg1, Intel8080::Register reg2);
    uint16_t getRegisterPairValue(Intel8080::RegisterPair regpair);
    std::pair<Intel8080::Register, Intel8080::Register> registerPairToStdPair(Intel8080::RegisterPair regpair);
    uint16_t combineBytes(uint8_t hb, uint8_t lb);
    void setRegisterValue(Intel8080::Register reg, uint8_t val);
    void setRegisterPairValue(Intel8080::RegisterPair, uint16_t val);
    void generateOpcodes(void);
    void executeInstruction(Opcode opcode);

    bool getFlag(Intel8080::Flag flag);
    void setFlag(Intel8080::Flag flag);
    void resetFlag(Intel8080::Flag flag);

    bool getFlag(int pos);
    void setFlag(int pos, int state);
    void setFlags(bool carry, bool parity, bool auxiliaryCarry, bool zero, bool sign, uint8_t old_val, uint8_t new_val);


    bool checkParity(uint8_t val);
    bool checkForCarry(uint8_t old_val, uint8_t new_val);
    bool checkForAuxiliaryCarry(uint8_t old_val, uint8_t new_val);
    bool conditionMet(Intel8080::Condition condition);

    void op_nop(void);

    template <Intel8080::Register reg>
    void op_inr(void);

    template<Intel8080::Register reg>
    void op_dcr(void);

    template <Intel8080::Register reg>
    void op_mvi(void);

    template <Intel8080::Register destination, Intel8080::Register source>
    void op_mov(void);

    void op_xchg(void);
    void op_xthl(void);

    void op_jmp(void);
    void op_call(void);
    void op_ret(void);

    template <Intel8080::Condition condition>
    void op_j(void);
    
    template <Intel8080::Condition condition>
    void op_c(void);

    template <Intel8080::Condition condition>
    void op_r(void);

    template <int n>
    void op_rst(void);

    template <Intel8080::Register reg1, Intel8080::Register reg2>
    void op_lxi(void);

    template <Intel8080::RegisterPair regpair>
    void op_lxi(void);

    void op_ani(void);
    void op_adi(void);

    template <Intel8080::Register reg>
    void op_add(void);

    template <Intel8080::Register reg>
    void op_adc(void);

    void op_cpi(void);

    template <Intel8080::Register reg>
    void op_sub(void);

    template <Intel8080::Register reg>
    void op_sbb(void);

    template <Intel8080::Register reg>
    void op_ana(void);

    template <Intel8080::Register reg>
    void op_xra(void);

    template <Intel8080::Register reg>
    void op_ora(void);

    template <Intel8080::Register reg>
    void op_cmp(void);

    void op_sta(void);

    template <Intel8080::RegisterPair regpair>
    void op_stax(void);

    template <Intel8080::Register reg1, Intel8080::Register reg2>
    void op_stax(void);

    template <Intel8080::RegisterPair regpair>
    void op_inx(void);

    template <Intel8080::RegisterPair regpair>
    void op_dcx(void);

    template <Intel8080::RegisterPair regpair>
    void op_push(void);

    template <Intel8080::RegisterPair regpair>
    void op_pop(void);

    template <Intel8080::RegisterPair regpair>
    void op_dad(void);

    void op_rlc(void);
    void op_rrc(void);
    void op_ral(void);
    void op_rar(void);

    void op_hlt(void);

    void op_dump(void);
    void op_term(void);
};

#endif
