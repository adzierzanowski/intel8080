#include "Intel8080.h"

Opcode::Opcode(const uint8_t number,
  const uint8_t length,
  const std::string mnemonic,
  const std::string fullName,
  func_t callback):
    number(number), length(length), mnemonic(mnemonic), fullName(fullName), callback(callback)
{
}

Intel8080::Intel8080()
{
  memory.resize(0x10000);
  generateOpcodes();

  // constant bits in FLAGS register
  setFlag(5, 0);
  setFlag(3, 0);
  setFlag(1, 1);
}

void Intel8080::execute()
{
  uint8_t opcode_n = memory[pc];
  Opcode opcode = opcodes[opcode_n];
  pc += opcode.length;

  executeInstruction(opcode);

  if (!terminateFlag)
    execute();
}

void Intel8080::executeInstruction(Opcode opcode)
{
  currentOpcode = &opcode;

  if (debugOutput)
    printf(
      "\033[1m0x%02x\033[0m: \033[38;5;13m%5s\033[0m %s\n",
      opcode.number,
      opcode.mnemonic.c_str(),
      opcode.fullName.c_str()
    );

  if (opcode.callback != nullptr)
    (this->*(opcode.callback))();
  else
    printf("No callback.\n");
}

void Intel8080::printMemory()
{
  printMemory(0x100, 0x80);
}

void Intel8080::printMemory(int a, int n)
{
  for (int i=a; i < a+n; i++)
  {
    if (i % 0x10 == 0)
      printf("\n %#02x: ", i);
    printf("%02x ", memory[i]);
  }
  printf("\n");
}

void Intel8080::printRegisters()
{
  printf("\n================\033[1m\033[38;5;12mREGISTERS\033[0m================\n");
  printf("\033[1m\033[38;5;8mA\033[0m: \033[38;5;12m0x%02x\033[0m | ", a);
  printf("\033[1m\033[38;5;8mB\033[0m: \033[38;5;12m0x%02x\033[0m | ", b);
  printf("\033[1m\033[38;5;8mC\033[0m: \033[38;5;12m0x%02x\033[0m | ", c);
  printf("\033[1m\033[38;5;8mD\033[0m: \033[38;5;12m0x%02x\033[0m\n", d);
  printf("\033[1m\033[38;5;8mE\033[0m: \033[38;5;12m0x%02x\033[0m | ", e);
  printf("\033[1m\033[38;5;8mH\033[0m: \033[38;5;12m0x%02x\033[0m | ", h);
  printf("\033[1m\033[38;5;8mL\033[0m: \033[38;5;12m0x%02x\033[0m      SZ A P C\n", l);

  printf("\033[1m\033[38;5;8mSP\033[0m: \033[38;5;12m0x%04x\033[0m | ", sp);
  printf("\033[1m\033[38;5;8mPC\033[0m: \033[38;5;12m0x%04x\033[0m | ", pc);
  printf("\033[1m\033[38;5;8mFlags\033[0m\033[38;5;12m: ");
  uint8_t mask = 0x80;
  while (true)
  {
    printf("%c", flags & mask ? '1' : '0');
    mask >>= 1;
    
    if (mask == 0)
    {
      printf("\033[0m\n");
      break;
    }
  }
  printf("=========================================\n\n");
}

void Intel8080::loadProgram(std::vector<uint8_t> prog, uint16_t address)
{
  int i = 0;
  for (uint8_t b : prog)
  {
    memory[address + i] = b;
    i++;
  }
}

uint8_t Intel8080::getRegisterValue(Intel8080::Register reg)
{
  switch (reg)
  {
    case Register::A: return a; break;
    case Register::B: return b; break;
    case Register::C: return c; break;
    case Register::D: return d; break;
    case Register::E: return e; break;
    case Register::L: return l; break;
    case Register::H: return h; break;
    case Register::M: return 0; break;
  }
}

uint16_t Intel8080::getRegisterPairValue(Intel8080::Register reg1, Intel8080::Register reg2)
{
  return combineBytes(getRegisterValue(reg1), getRegisterValue(reg2));
}

uint16_t Intel8080::combineBytes(uint8_t hb, uint8_t lb)
{
  return (hb << 8) | lb;
}

void Intel8080::setRegisterValue(Intel8080::Register reg, uint8_t val)
{
  switch (reg)
  {
    case Register::A: a = val; break;
    case Register::B: b = val; break;
    case Register::C: c = val; break;
    case Register::D: d = val; break;
    case Register::E: e = val; break;
    case Register::L: l = val; break;
    case Register::H: h = val; break;
    case Register::M: break;
  }
}

bool Intel8080::checkParity(uint8_t val)
{
  int acc = 0;

  for (uint16_t mask = 1; mask <= 0x80; mask <<= 1)
  {
    if (val & mask)
      acc++;
  }

  if (acc % 2 == 0)
    return true;
  return false;
}

bool Intel8080::checkForAuxiliaryCarry(uint8_t old_val, uint8_t new_val)
{
  if (old_val & 0x8 && new_val & 0x10)
    return true;
  return false;
}

void Intel8080::setProgramCounter(uint16_t address)
{
  pc = address;
}

void Intel8080::op_dump()
{
  printRegisters();
}

void Intel8080::op_term()
{
  terminateFlag = true;
}

void Intel8080::op_nop()
{
}

void Intel8080::op_jmp()
{
  pc = combineBytes(memory[pc - 1], memory[pc - 2]);
  execute();
}

bool Intel8080::conditionMet(Intel8080::Condition condition)
{
  bool conditionMet_ = false;
  switch (condition)
  {
    case Condition::CARRY_FLAG_NOT_SET: conditionMet_ = !getFlag(Flag::C); break;
    case Condition::CARRY_FLAG_SET: conditionMet_ = getFlag(Flag::C); break;
    case Condition::PARITY_FLAG_NOT_SET: conditionMet_ = !getFlag(Flag::P); break;
    case Condition::PARITY_FLAG_SET: conditionMet_ = getFlag(Flag::P); break;
    case Condition::ZERO_FLAG_NOT_SET: conditionMet_ = !getFlag(Flag::Z); break;
    case Condition::ZERO_FLAG_SET: conditionMet_ = getFlag(Flag::Z); break;
    case Condition::SIGN_FLAG_NOT_SET: conditionMet_ = !getFlag(Flag::S); break;
    case Condition::SIGN_FLAG_SET: conditionMet_ = getFlag(Flag::S); break;
  }
  return conditionMet_;
}


template <Intel8080::Condition condition>
void Intel8080::op_j(void)
{
  if (conditionMet(condition))
    op_jmp();
}


void Intel8080::op_xchg()
{
  uint8_t tmp1 = d;
  uint8_t tmp2 = e;

  d = h;
  e = l;
  h = tmp1;
  l = tmp2;
}

template <Intel8080::Register reg>
void Intel8080::op_inr()
{
  uint8_t old_val = getRegisterValue(reg);
  uint8_t new_val = old_val + 1;
  setRegisterValue(reg, new_val);
  setFlags(false, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_dcr()
{
  uint8_t old_val = getRegisterValue(reg);
  uint8_t new_val = old_val - 1;
  setRegisterValue(reg, new_val);
  setFlags(false, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_mvi()
{
  uint8_t val = memory[pc-1];
  setRegisterValue(reg, val);
}

template <Intel8080::Register destination, Intel8080::Register source>
void Intel8080::op_mov()
{
  setRegisterValue(destination, getRegisterValue(source));
}

bool Intel8080::getFlag(int pos)
{
  return static_cast<bool>(flags & (1 << pos));
}

void Intel8080::setFlag(int pos, int state)
{
  if (state)
    flags |= (1 << pos);
  else
    flags &= ~(1 << pos);
}

void Intel8080::setFlags(bool carry, bool parity, bool auxiliaryCarry, bool zero, bool sign, uint8_t old_val, uint8_t new_val)
{
  if (carry) {} // check for Carry flag
  if (parity) checkParity(new_val) ? setFlag(Flag::P) : resetFlag(Flag::P);
  if (auxiliaryCarry) checkForAuxiliaryCarry(old_val, new_val) ? setFlag(Flag::AC) : resetFlag(Flag::AC);
  if (zero) new_val == 0 ? setFlag(Flag::Z) : resetFlag(Flag::Z);
  if (sign) new_val & 0x80 ? setFlag(Flag::S) : resetFlag(Flag::S);
}

bool Intel8080::getFlag(Intel8080::Flag flag)
{
  return static_cast<bool>((flags & (1 << static_cast<uint8_t>(flag))));
}

void Intel8080::setFlag(Intel8080::Flag flag)
{
  setFlag(static_cast<uint8_t>(flag), 1);
}

void Intel8080::resetFlag(Intel8080::Flag flag)
{
  setFlag(static_cast<uint8_t>(flag), 0);
}

void Intel8080::op_call(void)
{
  uint16_t addr = combineBytes(memory[pc - 1], memory[pc - 2]);

  // BDOS call
  if (addr == 0x0005)
  {
    switch (c)
    {
      // P_TERMCPM - System reset
      case 0x00:
        op_term();
        break;

      // C_WRITE - Console output
      // E = ASCII character
      case 0x02:
        putchar(e);
        break;
      
      // C_WRITESTR - Output string
      // D:E = addres of the string (terminated with '$')
      case 0x09:
        uint16_t addr = getRegisterPairValue(Register::D, Register::E);

        while (memory[addr] != (uint8_t) '$')
          putchar(memory[addr++]);

        break;
    }
  }
}

template <Intel8080::Condition condition>
void Intel8080::op_c(void)
{
  if (conditionMet(condition))
    op_call();
}

template <Intel8080::Register reg1, Intel8080::Register reg2>
void Intel8080::op_lxi(void)
{
  setRegisterValue(reg2, memory[pc - 2]);
  setRegisterValue(reg1, memory[pc - 1]);
}

template <Intel8080::RegisterPair regpair>
void Intel8080::op_lxi(void)
{
  switch (regpair)
  {
    case RegisterPair::BC: op_lxi<Register::B, Register::C>(); break;
    case RegisterPair::DE: op_lxi<Register::D, Register::E>(); break;
    case RegisterPair::HL: op_lxi<Register::H, Register::L>(); break;
    case RegisterPair::SP:
      sp = combineBytes(memory[pc - 1], memory[pc - 2]);
      break;
  }
}

void Intel8080::op_ani(void)
{
  uint8_t old_val = a;
  uint8_t new_val = memory[pc - 1] & a;
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

void Intel8080::op_adi(void)
{
  uint8_t old_val = a;
  uint8_t new_val = memory[pc - 1] + a;
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_add(void)
{
  uint8_t old_val = a;
  uint8_t new_val = a + getRegisterValue(reg);
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

void Intel8080::generateOpcodes(void)
{
  opcodes.push_back(Opcode(0x00, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x01, 3, "lxi", "Load register pair B:C", &Intel8080::op_lxi<RegisterPair::BC>));
  opcodes.push_back(Opcode(0x02, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x03, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x04, 1, "inr", "Increment register B", &Intel8080::op_inr<Register::B>));
  opcodes.push_back(Opcode(0x05, 1, "dcr", "Decrement register B", &Intel8080::op_dcr<Register::B>));
  opcodes.push_back(Opcode(0x06, 2, "mvi", "Move immediate to B", &Intel8080::op_mvi<Register::B>));
  opcodes.push_back(Opcode(0x07, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x08, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x09, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0a, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0b, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0c, 1, "inr", "Increment register C", &Intel8080::op_inr<Register::C>));
  opcodes.push_back(Opcode(0x0d, 1, "dcr", "Decrement register C", &Intel8080::op_dcr<Register::C>));
  opcodes.push_back(Opcode(0x0e, 2, "mvi", "Move immediate to C", &Intel8080::op_mvi<Register::C>));
  opcodes.push_back(Opcode(0x0f, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x10, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x11, 3, "lxi", "Load register pair D:E", &Intel8080::op_lxi<RegisterPair::DE>));
  opcodes.push_back(Opcode(0x12, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x13, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x14, 1, "inr", "Increment register D", &Intel8080::op_inr<Register::D>));
  opcodes.push_back(Opcode(0x15, 1, "dcr", "Unknown instruction", &Intel8080::op_dcr<Register::D>));
  opcodes.push_back(Opcode(0x16, 2, "mvi", "Move immediate to D", &Intel8080::op_mvi<Register::D>));
  opcodes.push_back(Opcode(0x17, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x18, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x19, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1a, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1b, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1c, 1, "inr", "Increment register E", &Intel8080::op_inr<Register::E>));
  opcodes.push_back(Opcode(0x1d, 1, "dcr", "Decrement register E", &Intel8080::op_dcr<Register::E>));
  opcodes.push_back(Opcode(0x1e, 2, "mvi", "Move immediate to E", &Intel8080::op_mvi<Register::E>));
  opcodes.push_back(Opcode(0x1f, 1, "null", "Unknown instruction", nullptr));
  
  opcodes.push_back(Opcode(0x20, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x21, 3, "lxi", "Load register pair H:L", &Intel8080::op_lxi<RegisterPair::HL>));
  opcodes.push_back(Opcode(0x22, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x23, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x24, 1, "inr", "Increment register H", &Intel8080::op_inr<Register::H>));
  opcodes.push_back(Opcode(0x25, 1, "dcr", "Decrement register H", &Intel8080::op_dcr<Register::H>));
  opcodes.push_back(Opcode(0x26, 2, "mvi", "Move immediate to H", &Intel8080::op_mvi<Register::H>));
  opcodes.push_back(Opcode(0x27, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x28, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x29, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2a, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2b, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2c, 1, "inr", "Increment register L", &Intel8080::op_inr<Register::L>));
  opcodes.push_back(Opcode(0x2d, 1, "dcr", "Decrement register L", &Intel8080::op_dcr<Register::L>));
  opcodes.push_back(Opcode(0x2e, 2, "mvi", "Move immediate to L", &Intel8080::op_mvi<Register::L>));
  opcodes.push_back(Opcode(0x2f, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x30, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x31, 3, "lxi", "Load register pair SP", &Intel8080::op_lxi<RegisterPair::SP>));
  opcodes.push_back(Opcode(0x32, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x33, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x34, 1, "inr", "Increment memref at H:L", &Intel8080::op_inr<Register::M>));
  opcodes.push_back(Opcode(0x35, 1, "dcr", "Decrement memref at H:L", &Intel8080::op_dcr<Register::M>));
  opcodes.push_back(Opcode(0x36, 2, "mvi", "Move immediate to memref at H:L", &Intel8080::op_mvi<Register::M>));
  opcodes.push_back(Opcode(0x37, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x38, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x39, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3a, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3b, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3c, 1, "inr", "Increment register A", &Intel8080::op_inr<Register::A>));
  opcodes.push_back(Opcode(0x3d, 1, "dcr", "Decrement register A", &Intel8080::op_dcr<Register::A>));
  opcodes.push_back(Opcode(0x3e, 2, "mvi", "Move immediate to A", &Intel8080::op_mvi<Register::A>));
  opcodes.push_back(Opcode(0x3f, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x40, 1, "mov", "Move B to B", &Intel8080::op_mov<Register::B, Register::B>));
  opcodes.push_back(Opcode(0x41, 1, "mov", "Move C to B", &Intel8080::op_mov<Register::B, Register::C>));
  opcodes.push_back(Opcode(0x42, 1, "mov", "Move D to B", &Intel8080::op_mov<Register::B, Register::D>));
  opcodes.push_back(Opcode(0x43, 1, "mov", "Move E to B", &Intel8080::op_mov<Register::B, Register::E>));
  opcodes.push_back(Opcode(0x44, 1, "mov", "Move H to B", &Intel8080::op_mov<Register::B, Register::H>));
  opcodes.push_back(Opcode(0x45, 1, "mov", "Move L to B", &Intel8080::op_mov<Register::B, Register::L>));
  opcodes.push_back(Opcode(0x46, 1, "mov", "Move M to B", &Intel8080::op_mov<Register::B, Register::M>));
  opcodes.push_back(Opcode(0x47, 1, "mov", "Move A to B", &Intel8080::op_mov<Register::B, Register::A>));
  opcodes.push_back(Opcode(0x48, 1, "mov", "Move B to C", &Intel8080::op_mov<Register::C, Register::B>));
  opcodes.push_back(Opcode(0x49, 1, "mov", "Move C to C", &Intel8080::op_mov<Register::C, Register::C>));
  opcodes.push_back(Opcode(0x4a, 1, "mov", "Move D to C", &Intel8080::op_mov<Register::C, Register::D>));
  opcodes.push_back(Opcode(0x4b, 1, "mov", "Move E to C", &Intel8080::op_mov<Register::C, Register::E>));
  opcodes.push_back(Opcode(0x4c, 1, "mov", "Move H to C", &Intel8080::op_mov<Register::C, Register::H>));
  opcodes.push_back(Opcode(0x4d, 1, "mov", "Move L to C", &Intel8080::op_mov<Register::C, Register::L>));
  opcodes.push_back(Opcode(0x4e, 1, "mov", "Move M to C", &Intel8080::op_mov<Register::C, Register::M>));
  opcodes.push_back(Opcode(0x4f, 1, "mov", "Move A to C", &Intel8080::op_mov<Register::C, Register::A>));

  opcodes.push_back(Opcode(0x50, 1, "mov", "Move B to D", &Intel8080::op_mov<Register::D, Register::B>));
  opcodes.push_back(Opcode(0x51, 1, "mov", "Move C to D", &Intel8080::op_mov<Register::D, Register::C>));
  opcodes.push_back(Opcode(0x52, 1, "mov", "Move D to D", &Intel8080::op_mov<Register::D, Register::D>));
  opcodes.push_back(Opcode(0x53, 1, "mov", "Move E to D", &Intel8080::op_mov<Register::D, Register::E>));
  opcodes.push_back(Opcode(0x54, 1, "mov", "Move H to D", &Intel8080::op_mov<Register::D, Register::H>));
  opcodes.push_back(Opcode(0x55, 1, "mov", "Move L to D", &Intel8080::op_mov<Register::D, Register::L>));
  opcodes.push_back(Opcode(0x56, 1, "mov", "Move M to D", &Intel8080::op_mov<Register::D, Register::M>));
  opcodes.push_back(Opcode(0x57, 1, "mov", "Move A to D", &Intel8080::op_mov<Register::D, Register::A>));
  opcodes.push_back(Opcode(0x58, 1, "mov", "Move B to E", &Intel8080::op_mov<Register::E, Register::B>));
  opcodes.push_back(Opcode(0x59, 1, "mov", "Move C to E", &Intel8080::op_mov<Register::E, Register::C>));
  opcodes.push_back(Opcode(0x5a, 1, "mov", "Move D to E", &Intel8080::op_mov<Register::E, Register::D>));
  opcodes.push_back(Opcode(0x5b, 1, "mov", "Move E to E", &Intel8080::op_mov<Register::E, Register::E>));
  opcodes.push_back(Opcode(0x5c, 1, "mov", "Move H to E", &Intel8080::op_mov<Register::E, Register::H>));
  opcodes.push_back(Opcode(0x5d, 1, "mov", "Move L to E", &Intel8080::op_mov<Register::E, Register::L>));
  opcodes.push_back(Opcode(0x5e, 1, "mov", "Move M to E", &Intel8080::op_mov<Register::E, Register::M>));
  opcodes.push_back(Opcode(0x5f, 1, "mov", "Move A to E", &Intel8080::op_mov<Register::E, Register::A>));

  opcodes.push_back(Opcode(0x60, 1, "mov", "Move B to H", &Intel8080::op_mov<Register::H, Register::B>));
  opcodes.push_back(Opcode(0x61, 1, "mov", "Move C to H", &Intel8080::op_mov<Register::H, Register::C>));
  opcodes.push_back(Opcode(0x62, 1, "mov", "Move D to H", &Intel8080::op_mov<Register::H, Register::D>));
  opcodes.push_back(Opcode(0x63, 1, "mov", "Move E to H", &Intel8080::op_mov<Register::H, Register::E>));
  opcodes.push_back(Opcode(0x64, 1, "mov", "Move H to H", &Intel8080::op_mov<Register::H, Register::H>));
  opcodes.push_back(Opcode(0x65, 1, "mov", "Move L to H", &Intel8080::op_mov<Register::H, Register::L>));
  opcodes.push_back(Opcode(0x66, 1, "mov", "Move M to H", &Intel8080::op_mov<Register::H, Register::M>));
  opcodes.push_back(Opcode(0x67, 1, "mov", "Move A to H", &Intel8080::op_mov<Register::H, Register::A>));
  opcodes.push_back(Opcode(0x68, 1, "mov", "Move B to L", &Intel8080::op_mov<Register::L, Register::B>));
  opcodes.push_back(Opcode(0x69, 1, "mov", "Move C to L", &Intel8080::op_mov<Register::L, Register::C>));
  opcodes.push_back(Opcode(0x6a, 1, "mov", "Move D to L", &Intel8080::op_mov<Register::L, Register::D>));
  opcodes.push_back(Opcode(0x6b, 1, "mov", "Move E to L", &Intel8080::op_mov<Register::L, Register::E>));
  opcodes.push_back(Opcode(0x6c, 1, "mov", "Move H to L", &Intel8080::op_mov<Register::L, Register::H>));
  opcodes.push_back(Opcode(0x6d, 1, "mov", "Move L to L", &Intel8080::op_mov<Register::L, Register::L>));
  opcodes.push_back(Opcode(0x6e, 1, "mov", "Move M to L", &Intel8080::op_mov<Register::L, Register::M>));
  opcodes.push_back(Opcode(0x6f, 1, "mov", "Move A to L", &Intel8080::op_mov<Register::L, Register::A>));

  opcodes.push_back(Opcode(0x70, 1, "mov", "Move B to M", &Intel8080::op_mov<Register::M, Register::B>));
  opcodes.push_back(Opcode(0x71, 1, "mov", "Move C to M", &Intel8080::op_mov<Register::M, Register::C>));
  opcodes.push_back(Opcode(0x72, 1, "mov", "Move D to M", &Intel8080::op_mov<Register::M, Register::D>));
  opcodes.push_back(Opcode(0x73, 1, "mov", "Move E to M", &Intel8080::op_mov<Register::M, Register::E>));
  opcodes.push_back(Opcode(0x74, 1, "mov", "Move H to M", &Intel8080::op_mov<Register::M, Register::H>));
  opcodes.push_back(Opcode(0x75, 1, "mov", "Move L to M", &Intel8080::op_mov<Register::M, Register::L>));
  opcodes.push_back(Opcode(0x76, 1, "hlt", "Halt processor (unimplemented)", nullptr));
  opcodes.push_back(Opcode(0x77, 1, "mov", "Move A to M", &Intel8080::op_mov<Register::M, Register::A>));
  opcodes.push_back(Opcode(0x78, 1, "mov", "Move B to A", &Intel8080::op_mov<Register::A, Register::B>));
  opcodes.push_back(Opcode(0x79, 1, "mov", "Move C to A", &Intel8080::op_mov<Register::A, Register::C>));
  opcodes.push_back(Opcode(0x7a, 1, "mov", "Move D to A", &Intel8080::op_mov<Register::A, Register::D>));
  opcodes.push_back(Opcode(0x7b, 1, "mov", "Move E to A", &Intel8080::op_mov<Register::A, Register::E>));
  opcodes.push_back(Opcode(0x7c, 1, "mov", "Move H to A", &Intel8080::op_mov<Register::A, Register::H>));
  opcodes.push_back(Opcode(0x7d, 1, "mov", "Move L to A", &Intel8080::op_mov<Register::A, Register::L>));
  opcodes.push_back(Opcode(0x7e, 1, "mov", "Move M to A", &Intel8080::op_mov<Register::A, Register::M>));
  opcodes.push_back(Opcode(0x7f, 1, "mov", "Move A to A", &Intel8080::op_mov<Register::A, Register::A>));

  opcodes.push_back(Opcode(0x80, 1, "add", "Add B to A", &Intel8080::op_add<Register::B>));
  opcodes.push_back(Opcode(0x81, 1, "add", "Add C to A", &Intel8080::op_add<Register::C>));
  opcodes.push_back(Opcode(0x82, 1, "add", "Add D to A", &Intel8080::op_add<Register::D>));
  opcodes.push_back(Opcode(0x83, 1, "add", "Add E to A", &Intel8080::op_add<Register::E>));
  opcodes.push_back(Opcode(0x84, 1, "add", "Add H to A", &Intel8080::op_add<Register::H>));
  opcodes.push_back(Opcode(0x85, 1, "add", "Add L to A", &Intel8080::op_add<Register::L>));
  opcodes.push_back(Opcode(0x86, 1, "add", "Add M to A", &Intel8080::op_add<Register::M>));
  opcodes.push_back(Opcode(0x87, 1, "add", "Add A to A", &Intel8080::op_add<Register::A>));
  opcodes.push_back(Opcode(0x88, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x89, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8a, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8b, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8c, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8d, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8e, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8f, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x90, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x91, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x92, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x93, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x94, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x95, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x96, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x97, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x98, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x99, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9a, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9b, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9c, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9d, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9e, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9f, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xa0, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa1, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa2, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa3, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa4, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa5, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa6, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa7, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa8, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa9, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xaa, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xab, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xac, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xad, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xae, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xaf, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xb0, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb1, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb2, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb3, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb4, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb5, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb6, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb7, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb8, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb9, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xba, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbb, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbc, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbd, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbe, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbf, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xc0, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc1, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc2, 3, "jnz", "Jump if not zero", &Intel8080::op_j<Condition::ZERO_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xc3, 3, "jmp", "Unconditional jump", &Intel8080::op_jmp));
  opcodes.push_back(Opcode(0xc4, 3, "cnz", "Call if not zero", &Intel8080::op_c<Condition::ZERO_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xc5, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc6, 1, "adi", "Add immediate with A", &Intel8080::op_adi));
  opcodes.push_back(Opcode(0xc7, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc8, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc9, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xca, 3, "jz", "Jump if zero", &Intel8080::op_j<Condition::ZERO_FLAG_SET>));
  opcodes.push_back(Opcode(0xcb, 3, "jmp", "Unconditional jump", &Intel8080::op_jmp));
  opcodes.push_back(Opcode(0xcc, 3, "cz", "Call if zero", &Intel8080::op_c<Condition::ZERO_FLAG_SET>));
  opcodes.push_back(Opcode(0xcd, 3, "call", "Unconditional call", &Intel8080::op_call));
  opcodes.push_back(Opcode(0xce, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xcf, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xd0, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd1, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd2, 3, "jnc", "Jump if not carry", &Intel8080::op_j<Condition::CARRY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xd3, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd4, 3, "cnc", "Call if not carry", &Intel8080::op_c<Condition::CARRY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xd5, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd6, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd7, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd8, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd9, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xda, 3, "jc", "Jump if carry", &Intel8080::op_j<Condition::CARRY_FLAG_SET>));
  opcodes.push_back(Opcode(0xdb, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xdc, 3, "cc", "Call if carry", &Intel8080::op_c<Condition::CARRY_FLAG_SET>));
  opcodes.push_back(Opcode(0xdd, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xde, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xdf, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xe0, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe1, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe2, 3, "jpo", "Jump if parity flag odd", &Intel8080::op_j<Condition::PARITY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xe3, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe4, 3, "cpo", "Call if parity flag odd", &Intel8080::op_c<Condition::PARITY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xe5, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe6, 2, "ani", "And immediate with A", &Intel8080::op_ani));
  opcodes.push_back(Opcode(0xe7, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe8, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe9, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xea, 3, "jpe", "Jump if parity flag even", &Intel8080::op_j<Condition::PARITY_FLAG_SET>));
  opcodes.push_back(Opcode(0xeb, 1, "xchg", "Exchange registers D:E and H:L", &Intel8080::op_xchg));
  opcodes.push_back(Opcode(0xec, 3, "cpe", "Call if parity flag even", &Intel8080::op_j<Condition::PARITY_FLAG_SET>));
  opcodes.push_back(Opcode(0xed, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xee, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xef, 1, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xf0, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf1, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf2, 3, "jp", "Jump if positive", &Intel8080::op_j<Condition::SIGN_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xf3, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf4, 3, "cp", "Call if positive", &Intel8080::op_c<Condition::SIGN_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xf5, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf6, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf7, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf8, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf9, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xfa, 3, "jm", "Jump if negative", &Intel8080::op_j<Condition::SIGN_FLAG_SET>));
  opcodes.push_back(Opcode(0xfb, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xfc, 3, "cm", "Call if negative", &Intel8080::op_c<Condition::SIGN_FLAG_SET>));
  opcodes.push_back(Opcode(0xfd, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xfe, 1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xff, 1, "null", "Unknown instruction", nullptr));
}
