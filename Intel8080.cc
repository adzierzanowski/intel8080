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
  terminateFlag = false;
  uint8_t opcode_n = memory[pc];
  Opcode opcode = opcodes[opcode_n];
  pc += opcode.length;

  executeInstruction(opcode);

  if (!terminateFlag)
    execute();
}

void Intel8080::executeInstruction(Opcode opcode)
{
  if (debugOutput)
  {
    std::string fmt;
    if (formattedOutput)
      fmt = "%04x - \033[1m0x%02x\033[0m: \033[38;5;13m%5s\033[38;5;10m%5s\033[0m %s\n";
    else
      fmt = "%04x - 0x%02x: %5s%5s %s\n";

    char arg[16] = {'\0'};
    if (opcode.length == 2)
      sprintf(arg, " %02x", memory[pc-1]);
    else if (opcode.length == 3)
      sprintf(arg, " %04x", combineBytes(memory[pc-1], memory[pc-2]));
    printf(
      fmt.c_str(),
      pc - opcode.length,
      opcode.number,
      opcode.mnemonic.c_str(),
      arg,
      opcode.fullName.c_str()
    );
    if (verboseDebugOutput)
      printRegisters();
  }

  if (opcode.callback != nullptr)
    (this->*(opcode.callback))();
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
  std::string titlefmt = "";
  std::string endfmt = "";
  std::string regfmt = "";
  std::string valfmt = "";

  if (formattedOutput)
  {
    titlefmt = "\033[1m\033[38;5;12m";
    endfmt = "\033[0m";
    regfmt = "\033[1m\033[38;5;8m";
    valfmt = "\033[38;5;12m";
  }

  printf("\n================%sREGISTERS%s================\n", titlefmt.c_str(), endfmt.c_str());
  printf("%sA%s: %s0x%02x%s | ", regfmt.c_str(), endfmt.c_str(), valfmt.c_str(), a, endfmt.c_str());
  printf("%sB%s: %s0x%02x%s | ", regfmt.c_str(), endfmt.c_str(), valfmt.c_str(), b, endfmt.c_str());
  printf("%sC%s: %s0x%02x%s | ", regfmt.c_str(), endfmt.c_str(), valfmt.c_str(), c, endfmt.c_str());
  printf("%sD%s: %s0x%02x%s\n", regfmt.c_str(), endfmt.c_str(), valfmt.c_str(), d, endfmt.c_str());
  printf("%sE%s: %s0x%02x%s | ", regfmt.c_str(), endfmt.c_str(), valfmt.c_str(), e, endfmt.c_str());
  printf("%sH%s: %s0x%02x%s | ", regfmt.c_str(), endfmt.c_str(), valfmt.c_str(), h, endfmt.c_str());
  printf("%sL%s: %s0x%02x%s      SZ A P C\n", regfmt.c_str(), endfmt.c_str(), valfmt.c_str(), l, endfmt.c_str());

  printf("%sSP%s: %s0x%04x%s | ", regfmt.c_str(), endfmt.c_str(), valfmt.c_str(), sp, endfmt.c_str());
  printf("%sPC%s: %s0x%04x%s | ", regfmt.c_str(), endfmt.c_str(), valfmt.c_str(), pc, endfmt.c_str());
  printf("%sFlags%s: ", regfmt.c_str(), endfmt.c_str());
  uint8_t mask = 0x80;
  while (true)
  {
    printf("%c", flags & mask ? '1' : '0');
    mask >>= 1;
    
    if (mask == 0)
    {
      printf("%s\n", endfmt.c_str());
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

void Intel8080::loadProgram(std::string filename, uint16_t address)
{
  FILE *f = fopen(filename.c_str(), "rb");

  if (f == nullptr)
  {
    fprintf(stderr, "Error: fopen %s\n", filename.c_str());
    exit(1);
  }

  int b;
  std::vector<uint8_t> prog;
  while ((b = fgetc(f)) != EOF)
    prog.push_back(static_cast<uint8_t>(b));
  fclose(f);

  loadProgram(prog, address);
}

uint8_t Intel8080::getImmediate8(void)
{
  return memory[pc - 1];
}

uint16_t Intel8080::getImmediate16(void)
{
  return combineBytes(memory[pc - 1], memory[pc - 2]);
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
    case Register::M: return memory[getRegisterPairValue(RegisterPair::HL)]; break;
  }
}

std::pair<Intel8080::Register, Intel8080::Register> Intel8080::registerPairToStdPair(Intel8080::RegisterPair regpair)
{
  switch (regpair)
  {
    case RegisterPair::BC: return std::make_pair(Register::B, Register::C); break;
    case RegisterPair::DE: return std::make_pair(Register::D, Register::E); break;
    case RegisterPair::HL: return std::make_pair(Register::H, Register::L); break;
    case RegisterPair::SP: return std::make_pair(Register::M, Register::M); break; // nonsense but eh, TODO
  }
}

uint16_t Intel8080::getRegisterPairValue(Intel8080::Register reg1, Intel8080::Register reg2)
{
  return combineBytes(getRegisterValue(reg1), getRegisterValue(reg2));
}

uint16_t Intel8080::getRegisterPairValue(Intel8080::RegisterPair regpair)
{
  auto reg_stdpair = registerPairToStdPair(regpair);
  return getRegisterPairValue(reg_stdpair.first, reg_stdpair.second);
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
    case Register::M: memory[getRegisterPairValue(RegisterPair::HL)] = val; break;
  }
}

void Intel8080::setRegisterPairValue(Intel8080::RegisterPair regpair, uint16_t val)
{
  if (regpair == RegisterPair::SP)
  {
    sp = val;
  }
  else
  {
    auto reg_stdpair = registerPairToStdPair(regpair);
    setRegisterValue(reg_stdpair.first, val >> 8);
    setRegisterValue(reg_stdpair.second, val & 0x00ff);
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

bool Intel8080::checkForCarry(uint8_t old_val, uint16_t new_val)
{
  if (new_val > 0xff)
    return true;
  return false;
}

void Intel8080::setProgramCounter(uint16_t address)
{
  pc = address;
}

void Intel8080::setTerminateFlag()
{
  terminateFlag = true;
}

void Intel8080::op_nop()
{
}

void Intel8080::op_jmp()
{
  pc = getImmediate16();
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

void Intel8080::op_xthl(void)
{
  uint8_t hb = memory[sp + 1];
  uint8_t lb = memory[sp];

  memory[sp + 1] = h;
  memory[sp] = l;
  h = hb;
  l = lb;
}

template <Intel8080::Register reg>
void Intel8080::op_inr()
{
  uint8_t old_val = getRegisterValue(reg);
  uint16_t new_val = old_val + 1;
  setRegisterValue(reg, new_val);
  setFlags(false, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_dcr()
{
  uint8_t old_val = getRegisterValue(reg);
  uint16_t new_val = old_val - 1;
  setRegisterValue(reg, new_val);
  setFlags(false, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_mvi()
{
  uint8_t val = getImmediate8();
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

void Intel8080::setFlags(bool carry, bool parity, bool auxiliaryCarry, bool zero, bool sign, uint8_t old_val, uint16_t new_val)
{
  if (carry) checkForCarry(old_val, new_val) ? setFlag(Flag::C) : resetFlag(Flag::C);
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

  uint16_t addr = getImmediate16();

  if (addr == 0x0000)
    setTerminateFlag();
  
  else if (addr == 0x0005) // BDOS system calls
  {
    switch (c)
    {
      // BDOS function 0 (P_TERMCPM) - System Reset
      //   Entered with CL=0, DL=0 or 1. Does not return.
      //
      // Quit the current program, return to command prompt.
      // If DL is 0, the memory used by the program is deallocated;
      // if DL is 1, it remains resident.
      case 0x00:
        setTerminateFlag();
        if (d == 0x00 && l == 0x00)
          std::fill(memory.begin(), memory.end(), 0);
        break;
      
      // BDOS function 1 (C_READ) - Console input
      // BDOS function 3 (A_READ) - Auxiliary (Reader) input
      // BDOS function 6 (C_RAWIO) - Direct console I/O
      //   Returns A=L=character.
      //
      // Wait for a character from the keyboard; then echo it to the screen and return it.
      case 0x01:
      case 0x03:
      case 0x06:
        a = (uint8_t) getchar();
        l = a;
        break;

      // BDOS function 2 (C_WRITE) - Console output
      // BDOS function 4 (A_WRITE) - Auxiliary (Punch) output:
      // BDOS function 5 (L_WRITE) - Printer output
      //   E=ASCII character.
      //
      // Send the character in E to the screen.
      // Tabs are expanded to spaces.
      case 0x02:
      case 0x04:
      case 0x05:
        if((char) e == '\t')
          printf("  ");
        else
          putchar(e);
        break;
      
      // BDOS function 9 (C_WRITESTR) - Output string
      //   Entered with C=9, DE=address of string.
      //
      // Display a string of ASCII characters, terminated with the $ character.
      case 0x09:
        {
          uint16_t addr = getRegisterPairValue(RegisterPair::DE);
          while (memory[addr] != (uint8_t) stringDelimiter)
            putchar(memory[addr++]);
        }
        break;
      
      // BDOS function 10 (C_READSTR) - Buffered console input
      //   Entered with C=0x0a, DE=address or zero.
      //
      // This function reads characters from the keyboard into a memory buffer until RETURN is pressed.
      case 0x0a:
        {
          uint16_t addr = getRegisterPairValue(RegisterPair::DE);
          scanf("%[^\n]s", &memory[addr]);
        }
        break;

      // BDOS function 110 (C_DELIMIT) - Get/set string delimiter
      //   Entered with C=6Eh, DE=0FFFFh or ASCII value. Returns ASCII value in A.
      //
      // This function gets or sets the string delimiter.
      // If DE=0FFFFh, this gets the current ASCII value into A; otherwise it sets the delimiter to the value in E.
      case 0x6e:
        if (getRegisterPairValue(RegisterPair::DE) == 0xffff)
          a = (uint8_t) stringDelimiter;
        else
          stringDelimiter = (char) e;
        break;

      // CUSTOM function 0xfe - Print memory
      //  Entered with C=0xfe, DE=address, HL=length of memory region to print
      //
      // Prints emulator memory contents at a given address.
      case 0xfe:
        printMemory(getRegisterPairValue(RegisterPair::DE), getRegisterPairValue(RegisterPair::HL));
        break;
    
      // CUSTOM - Print registers
      case 0xff:
        printRegisters();
        break;
      
      default:
        break;
    }
  }
  else // not a BDOS call
  {
    // push current addr onto stack
    memory[sp - 1] = pc >> 8;
    memory[sp - 2] = pc & 0x00ff;
    sp -= 2;
    pc = addr; 
  }
}

void Intel8080::op_ret(void)
{
  // pop addr off the stack
  pc = combineBytes(memory[sp + 1], memory[sp]);
  sp += 2;
}
  
template <Intel8080::Condition condition>
void Intel8080::op_c(void)
{
  if (conditionMet(condition))
    op_call();
}

template <Intel8080::Condition condition>
void Intel8080::op_r(void)
{
  if (conditionMet(condition))
    op_ret();
}

template <int n>
void Intel8080::op_rst(void)
{
  // push current addr onto stack
  memory[sp - 1] = pc >> 8;
  memory[sp - 2] = pc & 0x00ff;
  sp -= 2;

  pc = n * 8;
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
      sp = getImmediate16();
      break;
  }
}

void Intel8080::op_ani(void)
{
  uint8_t old_val = a;
  uint16_t new_val = getImmediate8() & a;
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

void Intel8080::op_adi(void)
{
  uint8_t old_val = a;
  uint16_t new_val = getImmediate8();
  new_val += a;
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_add(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a + getRegisterValue(reg);
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_adc(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a + getRegisterValue(reg);
  setFlags(true, true, true, true, true, old_val, new_val);
  if (conditionMet(Condition::CARRY_FLAG_SET))
    a = new_val + 1;
  else
    a = new_val;
}

void Intel8080::op_cpi(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a - getImmediate8();

  setFlags(true, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_sub(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a - getRegisterValue(reg);
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_sbb(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a - getRegisterValue(reg) - static_cast<uint8_t>(getFlag(Flag::C));
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_ana(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a & getRegisterValue(reg);
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_xra(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a ^ getRegisterValue(reg);
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_ora(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a | getRegisterValue(reg);
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

template <Intel8080::Register reg>
void Intel8080::op_cmp(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a - getRegisterValue(reg);

  setFlags(true, true, true, true, true, old_val, new_val);
}

void Intel8080::op_sta(void)
{
  uint16_t addr = getImmediate16();
  memory[addr] = a;
}

template <Intel8080::RegisterPair regpair>
void Intel8080::op_stax(void)
{
  switch (regpair)
  {
    case RegisterPair::BC:
    case RegisterPair::DE:
      memory[getRegisterPairValue(regpair)] = a;
      break;

    default: break;
  }
}

template <Intel8080::RegisterPair regpair>
void Intel8080::op_inx(void)
{
  if (regpair == RegisterPair::SP)
    sp++;
  else
    setRegisterPairValue(regpair, getRegisterPairValue(regpair) + 1);
}

template <Intel8080::RegisterPair regpair>
void Intel8080::op_dcx(void)
{
  if (regpair == RegisterPair::SP)
    sp--;
  else
    setRegisterPairValue(regpair, getRegisterPairValue(regpair) - 1);
}

template <Intel8080::RegisterPair regpair>
void Intel8080::op_push(void)
{
  if (regpair == RegisterPair::SP)
  {
    memory[sp - 1] = flags;
    memory[sp - 2] = a;
  }
  else
  {
    auto reg_stdpair = registerPairToStdPair(regpair);
    memory[sp - 1] = getRegisterValue(reg_stdpair.first);
    memory[sp - 2] = getRegisterValue(reg_stdpair.second);
  }

  sp -= 2;
}

template <Intel8080::RegisterPair regpair>
void Intel8080::op_pop(void)
{
  uint8_t hb = memory[sp + 1];
  uint8_t lb = memory[sp];

  if (regpair == RegisterPair::SP)
  {
    flags = hb;
    a = lb;
  }
  else
  {
    setRegisterPairValue(regpair, combineBytes(hb, lb));
  }

  sp += 2;
}

void Intel8080::op_hlt(void)
{
  // ;)
  while (true);
}

void Intel8080::op_rlc(void)
{
  uint8_t old_val = a;
  uint8_t new_val = a << 1;
  a = new_val;

  bool leftmostBitSet = old_val & 0x80;
  leftmostBitSet ? setFlag(Flag::C) : resetFlag(Flag::C);
  leftmostBitSet ? a |= 1 : a &= ~(1);
}

void Intel8080::op_rrc(void)
{
  uint8_t old_val = a;
  uint8_t new_val = a >> 1;
  a = new_val;

  bool rightmostBitSet = old_val & 1;
  rightmostBitSet ? setFlag(Flag::C) : resetFlag(Flag::C);
  rightmostBitSet ? a |= 0x80 : a &= ~(0x80);
}

void Intel8080::op_ral(void)
{
  uint8_t carry = getFlag(Flag::C);
  uint8_t old_val = a;
  uint8_t new_val = (a << 1) | static_cast<uint8_t>(carry);
  a = new_val;
  
  old_val & 0x80 ? setFlag(Flag::C) : resetFlag(Flag::C);
}

void Intel8080::op_rar(void)
{
  uint8_t carry = getFlag(Flag::C);
  uint8_t old_val = a;
  uint8_t new_val = (a > 1) | (static_cast<uint8_t>(carry) << 7);
  a = new_val;
  
  old_val & 1 ? setFlag(Flag::C) : resetFlag(Flag::C);
}

template <Intel8080::RegisterPair regpair>
void Intel8080::op_dad(void)
{
  uint16_t old_val;
  if (regpair == RegisterPair::SP)
    old_val = sp;
  else
    old_val = getRegisterPairValue(regpair);
  
  uint16_t new_val = getRegisterPairValue(RegisterPair::HL) + old_val;
  setRegisterPairValue(RegisterPair::HL, new_val);
  setFlags(true, false, false, false, false, old_val, new_val);
}

template <Intel8080::RegisterPair regpair>
void Intel8080::op_ldax(void)
{
  switch (regpair)
  {
    case RegisterPair::BC:
      a = memory[getRegisterPairValue(RegisterPair::BC)];
      break;
    case RegisterPair::DE:
      a = memory[getRegisterPairValue(RegisterPair::DE)];
    default: break;
  }
}

void Intel8080::op_shld(void)
{
  uint16_t addr = getImmediate16();
  memory[addr] = getRegisterPairValue(RegisterPair::HL);
}

void Intel8080::op_lhld(void)
{
  uint16_t addr = getImmediate16();
  setRegisterPairValue(RegisterPair::HL, memory[addr]);
}

void Intel8080::op_cma(void)
{
  a = ~a;
}

void Intel8080::op_stc(void)
{
  setFlag(Flag::C);
}

void Intel8080::op_lda(void)
{
  uint16_t addr = getImmediate16();
  a = memory[addr];
}

void Intel8080::op_cmc(void)
{
  getFlag(Flag::C) ? resetFlag(Flag::C) : setFlag(Flag::C);
}

void Intel8080::op_aci(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a + getImmediate8() + static_cast<uint8_t>(getFlag(Flag::C));
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

void Intel8080::op_sui(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a - getImmediate8();
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

void Intel8080::op_sbi(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a - getImmediate8() - static_cast<uint8_t>(getFlag(Flag::C));
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

void Intel8080::op_pchl(void)
{
  pc = getRegisterPairValue(RegisterPair::HL);
}

void Intel8080::op_xri(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a ^ getImmediate8();
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

void Intel8080::op_ori(void)
{
  uint8_t old_val = a;
  uint16_t new_val = a | getImmediate8();
  a = new_val;
  setFlags(true, true, true, true, true, old_val, new_val);
}

void Intel8080::op_sphl(void)
{
  sp = getRegisterPairValue(RegisterPair::HL);
}

void Intel8080::generateOpcodes(void)
{
  opcodes.push_back(Opcode(0x00, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x01, 3, "lxi", "Load register pair B:C", &Intel8080::op_lxi<RegisterPair::BC>));
  opcodes.push_back(Opcode(0x02, 1, "stax", "Store A at [B:C]", &Intel8080::op_stax<RegisterPair::BC>));
  opcodes.push_back(Opcode(0x03, 1, "inx", "Increment register pair B:C", &Intel8080::op_inx<RegisterPair::BC>));
  opcodes.push_back(Opcode(0x04, 1, "inr", "Increment register B", &Intel8080::op_inr<Register::B>));
  opcodes.push_back(Opcode(0x05, 1, "dcr", "Decrement register B", &Intel8080::op_dcr<Register::B>));
  opcodes.push_back(Opcode(0x06, 2, "mvi", "Move immediate to B", &Intel8080::op_mvi<Register::B>));
  opcodes.push_back(Opcode(0x07, 1, "rlc", "Rotate A left", &Intel8080::op_rlc));
  opcodes.push_back(Opcode(0x08, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x09, 1, "dad", "Add B:C to H:L", &Intel8080::op_dad<RegisterPair::BC>));
  opcodes.push_back(Opcode(0x0a, 1, "ldax", "Load [B:C] to A", &Intel8080::op_ldax<RegisterPair::BC>));
  opcodes.push_back(Opcode(0x0b, 1, "dcx", "Decrement register pair B:C", &Intel8080::op_dcx<RegisterPair::BC>));
  opcodes.push_back(Opcode(0x0c, 1, "inr", "Increment register C", &Intel8080::op_inr<Register::C>));
  opcodes.push_back(Opcode(0x0d, 1, "dcr", "Decrement register C", &Intel8080::op_dcr<Register::C>));
  opcodes.push_back(Opcode(0x0e, 2, "mvi", "Move immediate to C", &Intel8080::op_mvi<Register::C>));
  opcodes.push_back(Opcode(0x0f, 1, "rrc", "Rotate A right", &Intel8080::op_rrc));

  opcodes.push_back(Opcode(0x10, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x11, 3, "lxi", "Load register pair D:E", &Intel8080::op_lxi<RegisterPair::DE>));
  opcodes.push_back(Opcode(0x12, 1, "stax", "Store A at [D:E]", &Intel8080::op_stax<RegisterPair::DE>));
  opcodes.push_back(Opcode(0x13, 1, "inx", "Increment register pair D:E", &Intel8080::op_inx<RegisterPair::DE>));
  opcodes.push_back(Opcode(0x14, 1, "inr", "Increment register D", &Intel8080::op_inr<Register::D>));
  opcodes.push_back(Opcode(0x15, 1, "dcr", "Unknown instruction", &Intel8080::op_dcr<Register::D>));
  opcodes.push_back(Opcode(0x16, 2, "mvi", "Move immediate to D", &Intel8080::op_mvi<Register::D>));
  opcodes.push_back(Opcode(0x17, 1, "ral", "Rotate left through carry", &Intel8080::op_ral));
  opcodes.push_back(Opcode(0x18, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x19, 1, "dad", "Add D:E to H:L", &Intel8080::op_dad<RegisterPair::DE>));
  opcodes.push_back(Opcode(0x1a, 1, "ldax", "Load A through [D:E]", &Intel8080::op_ldax<RegisterPair::DE>));
  opcodes.push_back(Opcode(0x1b, 1, "dcx", "Decrement register pair D:E", &Intel8080::op_dcx<RegisterPair::DE>));
  opcodes.push_back(Opcode(0x1c, 1, "inr", "Increment register E", &Intel8080::op_inr<Register::E>));
  opcodes.push_back(Opcode(0x1d, 1, "dcr", "Decrement register E", &Intel8080::op_dcr<Register::E>));
  opcodes.push_back(Opcode(0x1e, 2, "mvi", "Move immediate to E", &Intel8080::op_mvi<Register::E>));
  opcodes.push_back(Opcode(0x1f, 1, "rar", "Rotate A right through carry", &Intel8080::op_rar));
  
  opcodes.push_back(Opcode(0x20, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x21, 3, "lxi", "Load register pair H:L", &Intel8080::op_lxi<RegisterPair::HL>));
  opcodes.push_back(Opcode(0x22, 3, "shld", "Store H:L at [immediate]", &Intel8080::op_shld));
  opcodes.push_back(Opcode(0x23, 1, "inx", "Increment register pair H:L", &Intel8080::op_inx<RegisterPair::HL>));
  opcodes.push_back(Opcode(0x24, 1, "inr", "Increment register H", &Intel8080::op_inr<Register::H>));
  opcodes.push_back(Opcode(0x25, 1, "dcr", "Decrement register H", &Intel8080::op_dcr<Register::H>));
  opcodes.push_back(Opcode(0x26, 2, "mvi", "Move immediate to H", &Intel8080::op_mvi<Register::H>));
  opcodes.push_back(Opcode(0x27, 1, "daa", "Not implemented", nullptr));
  opcodes.push_back(Opcode(0x28, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x29, 1, "dad", "Add H:L to H:L", &Intel8080::op_dad<RegisterPair::HL>));
  opcodes.push_back(Opcode(0x2a, 3, "lhld", "Load H:L from [immediate]", &Intel8080::op_lhld));
  opcodes.push_back(Opcode(0x2b, 1, "dcx", "Decrement register pair H:L", &Intel8080::op_dcx<RegisterPair::HL>));
  opcodes.push_back(Opcode(0x2c, 1, "inr", "Increment register L", &Intel8080::op_inr<Register::L>));
  opcodes.push_back(Opcode(0x2d, 1, "dcr", "Decrement register L", &Intel8080::op_dcr<Register::L>));
  opcodes.push_back(Opcode(0x2e, 2, "mvi", "Move immediate to L", &Intel8080::op_mvi<Register::L>));
  opcodes.push_back(Opcode(0x2f, 1, "cma", "Compliment A", &Intel8080::op_cma));

  opcodes.push_back(Opcode(0x30, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x31, 3, "lxi", "Load register pair SP", &Intel8080::op_lxi<RegisterPair::SP>));
  opcodes.push_back(Opcode(0x32, 3, "sta", "Store A at [immediate]", &Intel8080::op_sta));
  opcodes.push_back(Opcode(0x33, 1, "inx", "Increment SP", &Intel8080::op_inx<RegisterPair::SP>));
  opcodes.push_back(Opcode(0x34, 1, "inr", "Increment memory at [H:L]", &Intel8080::op_inr<Register::M>));
  opcodes.push_back(Opcode(0x35, 1, "dcr", "Decrement memory at [H:L]", &Intel8080::op_dcr<Register::M>));
  opcodes.push_back(Opcode(0x36, 2, "mvi", "Move immediate to [H:L]", &Intel8080::op_mvi<Register::M>));
  opcodes.push_back(Opcode(0x37, 1, "stc", "Set carry flag", &Intel8080::op_stc));
  opcodes.push_back(Opcode(0x38, 1, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x39, 1, "dad", "Add SP to H:L", &Intel8080::op_dad<RegisterPair::SP>));
  opcodes.push_back(Opcode(0x3a, 3, "lda", "Load A from [immediate]", &Intel8080::op_lda));
  opcodes.push_back(Opcode(0x3b, 1, "dcx", "Decrement SP", &Intel8080::op_dcx<RegisterPair::SP>));
  opcodes.push_back(Opcode(0x3c, 1, "inr", "Increment register A", &Intel8080::op_inr<Register::A>));
  opcodes.push_back(Opcode(0x3d, 1, "dcr", "Decrement register A", &Intel8080::op_dcr<Register::A>));
  opcodes.push_back(Opcode(0x3e, 2, "mvi", "Move immediate to A", &Intel8080::op_mvi<Register::A>));
  opcodes.push_back(Opcode(0x3f, 1, "cmc", "Compilment carry flag", &Intel8080::op_cmc));

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
  opcodes.push_back(Opcode(0x76, 1, "hlt", "Halt processor", &Intel8080::op_hlt));
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
  opcodes.push_back(Opcode(0x88, 1, "adc", "Add with carry B to A", &Intel8080::op_adc<Register::B>));
  opcodes.push_back(Opcode(0x89, 1, "adc", "Add with carry C to A", &Intel8080::op_adc<Register::C>));
  opcodes.push_back(Opcode(0x8a, 1, "adc", "Add with carry D to A", &Intel8080::op_adc<Register::D>));
  opcodes.push_back(Opcode(0x8b, 1, "adc", "Add with carry E to A", &Intel8080::op_adc<Register::E>));
  opcodes.push_back(Opcode(0x8c, 1, "adc", "Add with carry H to A", &Intel8080::op_adc<Register::H>));
  opcodes.push_back(Opcode(0x8d, 1, "adc", "Add with carry L to A", &Intel8080::op_adc<Register::L>));
  opcodes.push_back(Opcode(0x8e, 1, "adc", "Add with carry M to A", &Intel8080::op_adc<Register::M>));
  opcodes.push_back(Opcode(0x8f, 1, "adc", "Add with carry A to A", &Intel8080::op_adc<Register::A>));

  opcodes.push_back(Opcode(0x90, 1, "sub", "Substract B from A", &Intel8080::op_sub<Register::B>));
  opcodes.push_back(Opcode(0x91, 1, "sub", "Substract C from A", &Intel8080::op_sub<Register::C>));
  opcodes.push_back(Opcode(0x92, 1, "sub", "Substract D from A", &Intel8080::op_sub<Register::D>));
  opcodes.push_back(Opcode(0x93, 1, "sub", "Substract E from A", &Intel8080::op_sub<Register::E>));
  opcodes.push_back(Opcode(0x94, 1, "sub", "Substract H from A", &Intel8080::op_sub<Register::H>));
  opcodes.push_back(Opcode(0x95, 1, "sub", "Substract L from A", &Intel8080::op_sub<Register::L>));
  opcodes.push_back(Opcode(0x96, 1, "sub", "Substract M from A", &Intel8080::op_sub<Register::M>));
  opcodes.push_back(Opcode(0x97, 1, "sub", "Substract A from A", &Intel8080::op_sub<Register::A>));
  opcodes.push_back(Opcode(0x98, 1, "sbb", "Substract with borrow B from A", &Intel8080::op_sbb<Register::B>));
  opcodes.push_back(Opcode(0x99, 1, "sbb", "Substract with borrow C from A", &Intel8080::op_sbb<Register::C>));
  opcodes.push_back(Opcode(0x9a, 1, "sbb", "Substract with borrow D from A", &Intel8080::op_sbb<Register::D>));
  opcodes.push_back(Opcode(0x9b, 1, "sbb", "Substract with borrow E from A", &Intel8080::op_sbb<Register::E>));
  opcodes.push_back(Opcode(0x9c, 1, "sbb", "Substract with borrow H from A", &Intel8080::op_sbb<Register::H>));
  opcodes.push_back(Opcode(0x9d, 1, "sbb", "Substract with borrow L from A", &Intel8080::op_sbb<Register::L>));
  opcodes.push_back(Opcode(0x9e, 1, "sbb", "Substract with borrow M from A", &Intel8080::op_sbb<Register::M>));
  opcodes.push_back(Opcode(0x9f, 1, "sbb", "Substract with borrow A from A", &Intel8080::op_sbb<Register::A>));

  opcodes.push_back(Opcode(0xa0, 1, "ana", "AND B with A", &Intel8080::op_ana<Register::B>));
  opcodes.push_back(Opcode(0xa1, 1, "ana", "AND C with A", &Intel8080::op_ana<Register::C>));
  opcodes.push_back(Opcode(0xa2, 1, "ana", "AND D with A", &Intel8080::op_ana<Register::D>));
  opcodes.push_back(Opcode(0xa3, 1, "ana", "AND E with A", &Intel8080::op_ana<Register::E>));
  opcodes.push_back(Opcode(0xa4, 1, "ana", "AND H with A", &Intel8080::op_ana<Register::H>));
  opcodes.push_back(Opcode(0xa5, 1, "ana", "AND L with A", &Intel8080::op_ana<Register::L>));
  opcodes.push_back(Opcode(0xa6, 1, "ana", "AND M with A", &Intel8080::op_ana<Register::M>));
  opcodes.push_back(Opcode(0xa7, 1, "ana", "AND A with A", &Intel8080::op_ana<Register::A>));
  opcodes.push_back(Opcode(0xa8, 1, "xra", "XOR B with A", &Intel8080::op_xra<Register::B>));
  opcodes.push_back(Opcode(0xa9, 1, "xra", "XOR C with A", &Intel8080::op_xra<Register::C>));
  opcodes.push_back(Opcode(0xaa, 1, "xra", "XOR D with A", &Intel8080::op_xra<Register::D>));
  opcodes.push_back(Opcode(0xab, 1, "xra", "XOR E with A", &Intel8080::op_xra<Register::E>));
  opcodes.push_back(Opcode(0xac, 1, "xra", "XOR H with A", &Intel8080::op_xra<Register::H>));
  opcodes.push_back(Opcode(0xad, 1, "xra", "XOR L with A", &Intel8080::op_xra<Register::L>));
  opcodes.push_back(Opcode(0xae, 1, "xra", "XOR M with A", &Intel8080::op_xra<Register::M>));
  opcodes.push_back(Opcode(0xaf, 1, "xra", "XOR A with A", &Intel8080::op_xra<Register::A>));

  opcodes.push_back(Opcode(0xb0, 1, "ora", "OR B with A", &Intel8080::op_ora<Register::B>));
  opcodes.push_back(Opcode(0xb1, 1, "ora", "OR C with A", &Intel8080::op_ora<Register::C>));
  opcodes.push_back(Opcode(0xb2, 1, "ora", "OR D with A", &Intel8080::op_ora<Register::D>));
  opcodes.push_back(Opcode(0xb3, 1, "ora", "OR E with A", &Intel8080::op_ora<Register::E>));
  opcodes.push_back(Opcode(0xb4, 1, "ora", "OR H with A", &Intel8080::op_ora<Register::H>));
  opcodes.push_back(Opcode(0xb5, 1, "ora", "OR L with A", &Intel8080::op_ora<Register::L>));
  opcodes.push_back(Opcode(0xb6, 1, "ora", "OR M with A", &Intel8080::op_ora<Register::M>));
  opcodes.push_back(Opcode(0xb7, 1, "ora", "OR A with A", &Intel8080::op_ora<Register::A>));
  opcodes.push_back(Opcode(0xb8, 1, "cmp", "Compare B with A", &Intel8080::op_cmp<Register::B>));
  opcodes.push_back(Opcode(0xb9, 1, "cmp", "Compare C with A", &Intel8080::op_cmp<Register::C>));
  opcodes.push_back(Opcode(0xba, 1, "cmp", "Compare D with A", &Intel8080::op_cmp<Register::D>));
  opcodes.push_back(Opcode(0xbb, 1, "cmp", "Compare E with A", &Intel8080::op_cmp<Register::E>));
  opcodes.push_back(Opcode(0xbc, 1, "cmp", "Compare H with A", &Intel8080::op_cmp<Register::H>));
  opcodes.push_back(Opcode(0xbd, 1, "cmp", "Compare L with A", &Intel8080::op_cmp<Register::L>));
  opcodes.push_back(Opcode(0xbe, 1, "cmp", "Compare M with A", &Intel8080::op_cmp<Register::M>));
  opcodes.push_back(Opcode(0xbf, 1, "cmp", "Compare A with A", &Intel8080::op_cmp<Register::A>));

  opcodes.push_back(Opcode(0xc0, 1, "rnz", "Return if not zero", &Intel8080::op_r<Condition::ZERO_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xc1, 1, "pop", "Pop to B:C off stack", &Intel8080::op_pop<RegisterPair::BC>));
  opcodes.push_back(Opcode(0xc2, 3, "jnz", "Jump if not zero", &Intel8080::op_j<Condition::ZERO_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xc3, 3, "jmp", "Unconditional jump", &Intel8080::op_jmp));
  opcodes.push_back(Opcode(0xc4, 3, "cnz", "Call if not zero", &Intel8080::op_c<Condition::ZERO_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xc5, 1, "push", "Push B:C onto stack", &Intel8080::op_push<RegisterPair::BC>));
  opcodes.push_back(Opcode(0xc6, 2, "adi", "Add immediate with A", &Intel8080::op_adi));
  opcodes.push_back(Opcode(0xc7, 1, "rst", "Reset 0", &Intel8080::op_rst<0>));
  opcodes.push_back(Opcode(0xc8, 1, "rz", "Return if zero", &Intel8080::op_r<Condition::ZERO_FLAG_SET>));
  opcodes.push_back(Opcode(0xc9, 1, "ret", "Return", &Intel8080::op_ret));
  opcodes.push_back(Opcode(0xca, 3, "jz", "Jump if zero", &Intel8080::op_j<Condition::ZERO_FLAG_SET>));
  opcodes.push_back(Opcode(0xcb, 3, "jmp", "Unconditional jump", &Intel8080::op_jmp));
  opcodes.push_back(Opcode(0xcc, 3, "cz", "Call if zero", &Intel8080::op_c<Condition::ZERO_FLAG_SET>));
  opcodes.push_back(Opcode(0xcd, 3, "call", "Unconditional call", &Intel8080::op_call));
  opcodes.push_back(Opcode(0xce, 2, "aci", "Add immediate to A with carry", &Intel8080::op_aci));
  opcodes.push_back(Opcode(0xcf, 1, "rst", "Reset 1", &Intel8080::op_rst<1>));

  opcodes.push_back(Opcode(0xd0, 1, "rnc", "Return if not carry", &Intel8080::op_r<Condition::CARRY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xd1, 1, "pop", "Pop to D:E off stack", &Intel8080::op_pop<RegisterPair::DE>));
  opcodes.push_back(Opcode(0xd2, 3, "jnc", "Jump if not carry", &Intel8080::op_j<Condition::CARRY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xd3, 2, "out", "Not implemented", nullptr));
  opcodes.push_back(Opcode(0xd4, 3, "cnc", "Call if not carry", &Intel8080::op_c<Condition::CARRY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xd5, 1, "push", "Push D:E onto stack", &Intel8080::op_push<RegisterPair::DE>));
  opcodes.push_back(Opcode(0xd6, 2, "sui", "Substract immediate from A", &Intel8080::op_sui));
  opcodes.push_back(Opcode(0xd7, 1, "rst", "Reset 2", &Intel8080::op_rst<2>));
  opcodes.push_back(Opcode(0xd8, 1, "rc", "Return if carry", &Intel8080::op_r<Condition::CARRY_FLAG_SET>));
  opcodes.push_back(Opcode(0xd9, 1, "ret", "Unconditional return", &Intel8080::op_ret));
  opcodes.push_back(Opcode(0xda, 3, "jc", "Jump if carry", &Intel8080::op_j<Condition::CARRY_FLAG_SET>));
  opcodes.push_back(Opcode(0xdb, 2, "in", "Not implemented", nullptr));
  opcodes.push_back(Opcode(0xdc, 3, "cc", "Call if carry", &Intel8080::op_c<Condition::CARRY_FLAG_SET>));
  opcodes.push_back(Opcode(0xdd, 3, "call", "Unconditional call", &Intel8080::op_call));
  opcodes.push_back(Opcode(0xde, 2, "sbi", "Substract immediate from A with borrow", &Intel8080::op_sbi));
  opcodes.push_back(Opcode(0xdf, 1, "rst", "Reset 3", &Intel8080::op_rst<3>));

  opcodes.push_back(Opcode(0xe0, 1, "rpo", "Return if parity odd", &Intel8080::op_r<Condition::PARITY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xe1, 1, "pop", "Pop to H:L off stack", &Intel8080::op_pop<RegisterPair::HL>));
  opcodes.push_back(Opcode(0xe2, 3, "jpo", "Jump if parity flag odd", &Intel8080::op_j<Condition::PARITY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xe3, 1, "xthl", "Exchange H:L with top word on the stack", &Intel8080::op_xthl));
  opcodes.push_back(Opcode(0xe4, 3, "cpo", "Call if parity flag odd", &Intel8080::op_c<Condition::PARITY_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xe5, 1, "push", "Push H:L onto stack", &Intel8080::op_push<RegisterPair::HL>));
  opcodes.push_back(Opcode(0xe6, 2, "ani", "And immediate with A", &Intel8080::op_ani));
  opcodes.push_back(Opcode(0xe7, 1, "rst", "Reset 4", &Intel8080::op_rst<4>));
  opcodes.push_back(Opcode(0xe8, 1, "rpe", "Return if parity even", &Intel8080::op_r<Condition::PARITY_FLAG_SET>));
  opcodes.push_back(Opcode(0xe9, 1, "pchl", "Jump to [H:L]", &Intel8080::op_pchl));
  opcodes.push_back(Opcode(0xea, 3, "jpe", "Jump if parity flag even", &Intel8080::op_j<Condition::PARITY_FLAG_SET>));
  opcodes.push_back(Opcode(0xeb, 1, "xchg", "Exchange registers D:E and H:L", &Intel8080::op_xchg));
  opcodes.push_back(Opcode(0xec, 3, "cpe", "Call if parity flag even", &Intel8080::op_j<Condition::PARITY_FLAG_SET>));
  opcodes.push_back(Opcode(0xed, 3, "call", "Unconditional call", &Intel8080::op_call));
  opcodes.push_back(Opcode(0xee, 2, "xri", "XOR immediate with A", &Intel8080::op_xri));
  opcodes.push_back(Opcode(0xef, 1, "rst", "Reset 5", &Intel8080::op_rst<5>));

  opcodes.push_back(Opcode(0xf0, 1, "rp", "Return if positive", &Intel8080::op_r<Condition::SIGN_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xf1, 1, "pop", "Pop to FLAGS:A off stack", &Intel8080::op_pop<RegisterPair::SP>));
  opcodes.push_back(Opcode(0xf2, 3, "jp", "Jump if positive", &Intel8080::op_j<Condition::SIGN_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xf3, 1, "di", "Not implemented", nullptr));
  opcodes.push_back(Opcode(0xf4, 3, "cp", "Call if positive", &Intel8080::op_c<Condition::SIGN_FLAG_NOT_SET>));
  opcodes.push_back(Opcode(0xf5, 1, "push", "Push FLAGS:A onto stack", &Intel8080::op_push<RegisterPair::SP>));
  opcodes.push_back(Opcode(0xf6, 2, "ori", "OR immediate with A", &Intel8080::op_ori));
  opcodes.push_back(Opcode(0xf7, 1, "rst", "Reset 6", &Intel8080::op_rst<6>));
  opcodes.push_back(Opcode(0xf8, 1, "rm", "Return if negative", &Intel8080::op_r<Condition::SIGN_FLAG_SET>));
  opcodes.push_back(Opcode(0xf9, 1, "sphl", "Set SP to H:L", &Intel8080::op_sphl));
  opcodes.push_back(Opcode(0xfa, 3, "jm", "Jump if negative", &Intel8080::op_j<Condition::SIGN_FLAG_SET>));
  opcodes.push_back(Opcode(0xfb, 1, "ei", "Not implemented", nullptr));
  opcodes.push_back(Opcode(0xfc, 3, "cm", "Call if negative", &Intel8080::op_c<Condition::SIGN_FLAG_SET>));
  opcodes.push_back(Opcode(0xfd, 3, "call", "Unconditional call", &Intel8080::op_call));
  opcodes.push_back(Opcode(0xfe, 2, "cpi", "Compare immediate with A", &Intel8080::op_cpi));
  opcodes.push_back(Opcode(0xff, 1, "rst", "Reset 7", &Intel8080::op_rst<7>));
}
