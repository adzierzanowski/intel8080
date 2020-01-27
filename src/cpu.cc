#include "cpu.hh"


bool CPU::aux_carry(uint8_t before, uint8_t after)
{
  uint8_t a = before;
  uint8_t b = after - before;
  a &= 0x0f;
  b &= 0x0f;
  return (a+b) >> 4;
}

bool CPU::even_parity(uint8_t val)
{
  int acc = 0;
  for (int i = 0; i < 8; i++)
  {
    acc += val & 1;
    val >>= 1;
  }
  return acc % 2 == 0;
}

CPU::CPU(void) :
  a{0}, b{0}, c{0}, d{0}, h{0}, l{0},
  sp{0}, pc{0}, flags{0b00000010},
  ram{std::make_unique<RAM>()}
{
}

void CPU::load_program(std::vector<uint8_t> prog, uint16_t addr)
{
  ram->set_values(prog, addr);
  pc = addr;
}

size_t CPU::get_memory_size(void)
{
  return ram->get_size();
}

uint8_t CPU::get_opcode(void)
{
  return ram->get(pc);
}

uint8_t CPU::get_register(Register x)
{
  switch (x)
  {
    case Register::A: return a; break; 
    case Register::B: return b; break; 
    case Register::C: return c; break; 
    case Register::D: return d; break; 
    case Register::E: return e; break; 
    case Register::H: return h; break; 
    case Register::L: return l; break; 
    case Register::M: return load(h<<8|l); break;
    case Register::FLAGS: return flags; break;
    default: return 0; break;
  }
}

void CPU::set_register(Register x, uint8_t value)
{
  switch (x)
  {
    case Register::A: a = value; break; 
    case Register::B: b = value; break; 
    case Register::C: c = value; break; 
    case Register::D: d = value; break; 
    case Register::E: e = value; break; 
    case Register::H: h = value; break; 
    case Register::L: l = value; break; 
    case Register::M: store(h<<8|l, value); break;
    case Register::FLAGS: flags = value; break;
    default: break;
  }
}

uint16_t CPU::get_register_pair(Register x, Register y)
{
  return get_register(x) << 8 | get_register(y);
}

void CPU::set_register_pair(Register x, Register y, uint16_t value)
{
  uint8_t hb = value >> 8;
  uint8_t lb = value & 0xff;
  set_register(x, hb);
  set_register(y, lb);
}

uint8_t CPU::get_imm8(void)
{
  return ram->get(pc+1);
}

uint16_t CPU::get_imm16(void)
{
  return ram->get(pc+2) << 8 | ram->get(pc+1);
}

void CPU::increment_pc(uint16_t by)
{
  pc += by;
}

void CPU::store(uint16_t addr, uint8_t value)
{
  ram->set(addr, value);
}

uint8_t CPU::load(uint16_t addr)
{
  return ram->get(addr);
}

uint16_t CPU::get_pc(void)
{
  return pc;
}

void CPU::set_pc(uint16_t addr)
{
  pc = addr;
}

uint16_t CPU::get_sp(void)
{
  return sp;
}

void CPU::set_sp(uint16_t value)
{
  sp = value;
}

void CPU::increment_sp(uint16_t by)
{
  sp += by;
}

void CPU::decrement_sp(uint16_t by)
{
  sp -= by;
}

bool CPU::get_flag(Flag f)
{
  return flags & static_cast<uint8_t>(f);
}

void CPU::set_flag(Flag f, bool set)
{
  if (f != Flag::Z1 && f != Flag::Z2 && f != Flag::O)
  {
    if (set)
      flags |= static_cast<uint8_t>(f);
    else
      flags &= ~(static_cast<uint8_t>(f));
  }
}

void CPU::affect_flags_szap(uint8_t before, uint8_t after)
{
  set_flag(Flag::S, after >= 0x7f);
  set_flag(Flag::Z, after == 0);
  set_flag(Flag::AC, CPU::aux_carry(before, after));
  set_flag(Flag::P, CPU::even_parity(after));
}

void CPU::push(uint16_t val)
{
  store(sp - 1, val >> 8);
  store(sp - 2, val & 0x00ff);
  sp -= 2;
}

uint16_t CPU::pop(void)
{
  sp += 2;
  return (load(sp + 1 - 2) << 8) | load(sp - 2);
}
