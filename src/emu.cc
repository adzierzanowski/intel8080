#include "emu.hh"

// TODO: relative to exe location
const std::string Emulator::opcodes_filename = "opcodes.csv";

Emulator::Emulator(void) :
  cpu{std::make_unique<CPU>()},
  opcodes{std::vector<Opcode>()},
  execute_flag{false},
  verbose_execution{false}
{
  FileLoader fl(FileLoader::OPCODES_FILENAME);
  fl.load_opcodes();
  opcodes = fl.get_opcodes();
}

void Emulator::load_program(std::vector<uint8_t> prog, uint16_t address)
{
  cpu->load_program(prog, address);
}

void Emulator::execute(void)
{
  execute_flag = true;
  uint8_t opcode = cpu->get_opcode();
  execute_opcode(opcode);

  if (execute_flag)
    execute();
}

std::string Emulator::dump_gpr(void)
{
  std::stringstream out;

  out << "A:" << boost::format("%02x") % +cpu->get_register(Register::A) << " ";
  out << "B:" << boost::format("%02x") % +cpu->get_register(Register::B) << " ";
  out << "C:" << boost::format("%02x") % +cpu->get_register(Register::C) << " ";
  out << "D:" << boost::format("%02x") % +cpu->get_register(Register::D) << " ";
  out << "E:" << boost::format("%02x") % +cpu->get_register(Register::E) << " ";
  out << "H:" << boost::format("%02x") % +cpu->get_register(Register::H) << " ";
  out << "L:" << boost::format("%02x") % +cpu->get_register(Register::L);

  return out.str();
}

std::string Emulator::dump_state_registers(void)
{
  std::stringstream out;

  out << "[PC:" << boost::format("%04x") % cpu->get_pc() << " ";
  out << "SP:" << boost::format("%04x->%04x")
    % cpu->get_sp()
    % (cpu->load(cpu->get_sp() + 1) << 8 | cpu->load(cpu->get_sp()));

  out << " FLAGS:" << std::bitset<8>(cpu->get_register(Register::FLAGS)) << "]";

  return out.str();
}

void Emulator::set_verbose_execution(bool flag)
{
  verbose_execution = flag;
}

void Emulator::execute_opcode(uint8_t opcode)
{
  auto op = opcodes[opcode];

  if (verbose_execution)
  {
    std::cout << dump_state_registers() << "\n";
    std::cout << dump_gpr() << "\n";
    std::cout << boost::format("%02x %-5s %-20s")
      % +opcode % op.mnemonic % op.description << "\n\n";
  }

  bool inc_pc = true;

  switch (opcode)
  {
    case 0x00:
    case 0x08:
    case 0x10:
    case 0x18:
    case 0x20:
    case 0x28:
    case 0x30:
    case 0x38:
      nop();
      break;

    case 0x01: lxi(Register::B, Register::C); break;
    case 0x02: stax(Register::B, Register::C); break;
    case 0x03: inx(Register::B, Register::C); break;
    case 0x04: inr(Register::B); break;
    case 0x05: dcr(Register::B); break;
    case 0x06: mvi(Register::B); break;
    case 0x07: rlc(); break;
    case 0x09: dad(Register::H, Register::L, Register::B, Register::C); break;
    case 0x0a: ldax(Register::B, Register::C); break;
    case 0x0b: dcx(Register::B, Register::C); break;
    case 0x0c: inr(Register::C); break;
    case 0x0d: dcr(Register::C); break;
    case 0x0e: mvi(Register::C); break;
    case 0x0f: rrc(); break;

    case 0x11: lxi(Register::D, Register::E); break;
    case 0x12: stax(Register::D, Register::E); break;
    case 0x13: inx(Register::D, Register::E); break;
    case 0x14: inr(Register::D); break;
    case 0x15: dcr(Register::D); break;
    case 0x16: mvi(Register::D); break;
    case 0x17: ral(); break;
    case 0x19: dad(Register::H, Register::L, Register::D, Register::E); break;
    case 0x1a: ldax(Register::D, Register::E); break;
    case 0x1b: dcx(Register::D, Register::E); break;
    case 0x1c: inr(Register::E); break;
    case 0x1d: dcr(Register::E); break;
    case 0x1e: mvi(Register::E); break;
    case 0x1f: rar(); break;

    case 0x21: lxi(Register::H, Register::L); break;
    case 0x22: shld(); break;
    case 0x23: inx(Register::H, Register::L); break;
    case 0x24: inr(Register::H); break;
    case 0x25: dcr(Register::H); break;
    case 0x26: mvi(Register::H); break;
    case 0x27: daa(); break;
    case 0x29: dad(Register::H, Register::L, Register::H, Register::L); break;
    case 0x2a: lhld(); break;
    case 0x2b: dcx(Register::H, Register::L); break;
    case 0x2c: inr(Register::L); break;
    case 0x2d: dcr(Register::L); break;
    case 0x2e: mvi(Register::L); break;
    case 0x2f: cma(); break;

    case 0x31: lxi(Register::SP); break;
    case 0x32: sta(); break;
    case 0x33: inx(Register::SP); break;
    case 0x34: inr(Register::M); break;
    case 0x35: dcr(Register::M); break;
    case 0x36: mvi(Register::M); break;
    case 0x37: stc(); break;
    case 0x39: dad(Register::H, Register::L, Register::SP); break;
    case 0x3a: lda(); break;
    case 0x3b: dcx(Register::SP); break;
    case 0x3c: inr(Register::A); break;
    case 0x3d: dcr(Register::A); break;
    case 0x3e: mvi(Register::A); break;
    case 0x3f: cmc(); break;

    case 0x40: mov(Register::B, Register::B); break;
    case 0x41: mov(Register::B, Register::C); break;
    case 0x42: mov(Register::B, Register::D); break;
    case 0x43: mov(Register::B, Register::E); break;
    case 0x44: mov(Register::B, Register::H); break;
    case 0x45: mov(Register::B, Register::L); break;
    case 0x46: mov(Register::B, Register::M); break;
    case 0x47: mov(Register::B, Register::A); break;
    case 0x48: mov(Register::C, Register::B); break;
    case 0x49: mov(Register::C, Register::C); break;
    case 0x4a: mov(Register::C, Register::D); break;
    case 0x4b: mov(Register::C, Register::E); break;
    case 0x4c: mov(Register::C, Register::H); break;
    case 0x4d: mov(Register::C, Register::L); break;
    case 0x4e: mov(Register::C, Register::M); break;
    case 0x4f: mov(Register::C, Register::A); break;

    case 0x50: mov(Register::D, Register::B); break;
    case 0x51: mov(Register::D, Register::C); break;
    case 0x52: mov(Register::D, Register::D); break;
    case 0x53: mov(Register::D, Register::E); break;
    case 0x54: mov(Register::D, Register::H); break;
    case 0x55: mov(Register::D, Register::L); break;
    case 0x56: mov(Register::D, Register::M); break;
    case 0x57: mov(Register::D, Register::A); break;
    case 0x58: mov(Register::E, Register::B); break;
    case 0x59: mov(Register::E, Register::C); break;
    case 0x5a: mov(Register::E, Register::D); break;
    case 0x5b: mov(Register::E, Register::E); break;
    case 0x5c: mov(Register::E, Register::H); break;
    case 0x5d: mov(Register::E, Register::L); break;
    case 0x5e: mov(Register::E, Register::M); break;
    case 0x5f: mov(Register::E, Register::A); break;

    case 0x60: mov(Register::H, Register::B); break;
    case 0x61: mov(Register::H, Register::C); break;
    case 0x62: mov(Register::H, Register::D); break;
    case 0x63: mov(Register::H, Register::E); break;
    case 0x64: mov(Register::H, Register::H); break;
    case 0x65: mov(Register::H, Register::L); break;
    case 0x66: mov(Register::H, Register::M); break;
    case 0x67: mov(Register::H, Register::A); break;
    case 0x68: mov(Register::L, Register::B); break;
    case 0x69: mov(Register::L, Register::C); break;
    case 0x6a: mov(Register::L, Register::D); break;
    case 0x6b: mov(Register::L, Register::E); break;
    case 0x6c: mov(Register::L, Register::H); break;
    case 0x6d: mov(Register::L, Register::L); break;
    case 0x6e: mov(Register::L, Register::M); break;
    case 0x6f: mov(Register::L, Register::A); break;

    case 0x70: mov(Register::M, Register::B); break;
    case 0x71: mov(Register::M, Register::C); break;
    case 0x72: mov(Register::M, Register::D); break;
    case 0x73: mov(Register::M, Register::E); break;
    case 0x74: mov(Register::M, Register::H); break;
    case 0x75: mov(Register::M, Register::L); break;
    case 0x76: execute_flag = false; break;
    case 0x77: mov(Register::M, Register::A); break;
    case 0x78: mov(Register::A, Register::B); break;
    case 0x79: mov(Register::A, Register::C); break;
    case 0x7a: mov(Register::A, Register::D); break;
    case 0x7b: mov(Register::A, Register::E); break;
    case 0x7c: mov(Register::A, Register::H); break;
    case 0x7d: mov(Register::A, Register::L); break;
    case 0x7e: mov(Register::A, Register::M); break;
    case 0x7f: mov(Register::A, Register::A); break;

    case 0x80: add(Register::B); break;
    case 0x81: add(Register::C); break;
    case 0x82: add(Register::D); break;
    case 0x83: add(Register::E); break;
    case 0x84: add(Register::H); break;
    case 0x85: add(Register::L); break;
    case 0x86: add(Register::M); break;
    case 0x87: add(Register::A); break;
    case 0x88: adc(Register::B); break;
    case 0x89: adc(Register::C); break;
    case 0x8a: adc(Register::D); break;
    case 0x8b: adc(Register::E); break;
    case 0x8c: adc(Register::H); break;
    case 0x8d: adc(Register::L); break;
    case 0x8e: adc(Register::M); break;
    case 0x8f: adc(Register::A); break;

    case 0x90: sub(Register::B); break;
    case 0x91: sub(Register::C); break;
    case 0x92: sub(Register::D); break;
    case 0x93: sub(Register::E); break;
    case 0x94: sub(Register::H); break;
    case 0x95: sub(Register::L); break;
    case 0x96: sub(Register::M); break;
    case 0x97: sub(Register::A); break;
    case 0x98: sbb(Register::B); break;
    case 0x99: sbb(Register::C); break;
    case 0x9a: sbb(Register::D); break;
    case 0x9b: sbb(Register::E); break;
    case 0x9c: sbb(Register::H); break;
    case 0x9d: sbb(Register::L); break;
    case 0x9e: sbb(Register::M); break;
    case 0x9f: sbb(Register::A); break;

    case 0xa0: ana(Register::B); break;
    case 0xa1: ana(Register::C); break;
    case 0xa2: ana(Register::D); break;
    case 0xa3: ana(Register::E); break;
    case 0xa4: ana(Register::H); break;
    case 0xa5: ana(Register::L); break;
    case 0xa6: ana(Register::M); break;
    case 0xa7: ana(Register::A); break;
    case 0xa8: xra(Register::B); break;
    case 0xa9: xra(Register::C); break;
    case 0xaa: xra(Register::D); break;
    case 0xab: xra(Register::E); break;
    case 0xac: xra(Register::H); break;
    case 0xad: xra(Register::L); break;
    case 0xae: xra(Register::M); break;
    case 0xaf: xra(Register::A); break;

    case 0xb0: ora(Register::B); break;
    case 0xb1: ora(Register::C); break;
    case 0xb2: ora(Register::D); break;
    case 0xb3: ora(Register::E); break;
    case 0xb4: ora(Register::H); break;
    case 0xb5: ora(Register::L); break;
    case 0xb6: ora(Register::M); break;
    case 0xb7: ora(Register::A); break;
    case 0xb8: cmp(Register::B); break;
    case 0xb9: cmp(Register::C); break;
    case 0xba: cmp(Register::D); break;
    case 0xbb: cmp(Register::E); break;
    case 0xbc: cmp(Register::H); break;
    case 0xbd: cmp(Register::L); break;
    case 0xbe: cmp(Register::M); break;
    case 0xbf: cmp(Register::A); break;

    case 0xc0: inc_pc = ret(!cpu->get_flag(Flag::Z)); break;
    case 0xc1: pop(Register::B, Register::C); break;
    case 0xc2: inc_pc = jmp(!cpu->get_flag(Flag::Z)); break;
    case 0xc3: inc_pc = jmp(true); break;
    case 0xc4: inc_pc = call(!cpu->get_flag(Flag::Z)); break;
    case 0xc5: push(Register::B, Register::C); break;
    case 0xc6: adi(); break;
    case 0xc7: rst(0); inc_pc = false; break;
    case 0xc8: inc_pc = ret(cpu->get_flag(Flag::Z)); break;
    case 0xc9: inc_pc = ret(true); break;
    case 0xca: inc_pc = jmp(cpu->get_flag(Flag::Z)); break;
    case 0xcb: inc_pc = jmp(true); break;
    case 0xcc: inc_pc = call(cpu->get_flag(Flag::Z)); break;
    case 0xcd: inc_pc = call(true); break;
    case 0xce: aci(); break;
    case 0xcf: rst(1); inc_pc = false; break;

    case 0xd0: inc_pc = ret(!cpu->get_flag(Flag::C)); break;
    case 0xd1: pop(Register::D, Register::E); break;
    case 0xd2: inc_pc = jmp(!cpu->get_flag(Flag::C)); break;
    case 0xd3: out(); break;
    case 0xd4: inc_pc = call(!cpu->get_flag(Flag::C)); break;
    case 0xd5: push(Register::D, Register::E); break;
    case 0xd6: sui(); break;
    case 0xd7: rst(2); inc_pc = false; break;
    case 0xd8: inc_pc = ret(cpu->get_flag(Flag::C)); break;
    case 0xd9: inc_pc = ret(true); break;
    case 0xda: inc_pc = jmp(cpu->get_flag(Flag::C)); break;
    case 0xdb: in(); break;
    case 0xdc: inc_pc = call(cpu->get_flag(Flag::C)); break;
    case 0xdd: inc_pc = call(true); break;
    case 0xde: sbi(); break;
    case 0xdf: rst(3); inc_pc = false; break;

    case 0xe0: inc_pc = ret(!cpu->get_flag(Flag::P)); break;
    case 0xe1: pop(Register::H, Register::L); break;
    case 0xe2: inc_pc = jmp(!cpu->get_flag(Flag::P)); break;
    case 0xe3: xthl(); break;
    case 0xe4: inc_pc = call(!cpu->get_flag(Flag::P)); break;
    case 0xe5: push(Register::H, Register::L); break;
    case 0xe6: ani(); break;
    case 0xe7: rst(4); inc_pc = false; break;
    case 0xe8: inc_pc = ret(cpu->get_flag(Flag::P)); break;
    case 0xe9: pchl(); inc_pc = false; break;
    case 0xea: inc_pc = jmp(cpu->get_flag(Flag::P)); break;
    case 0xeb: xchg(); break;
    case 0xec: inc_pc = call(cpu->get_flag(Flag::P)); break;
    case 0xed: inc_pc = call(true); break;
    case 0xee: xri(); break;
    case 0xef: rst(5); inc_pc = false; break;

    case 0xf0: inc_pc = ret(!cpu->get_flag(Flag::S)); break;
    case 0xf1: pop(Register::A, Register::FLAGS); break;
    case 0xf2: inc_pc = jmp(!cpu->get_flag(Flag::S)); break;
    case 0xf3: di(); break;
    case 0xf4: inc_pc = call(!cpu->get_flag(Flag::S)); break;
    case 0xf5: push(Register::A, Register::FLAGS); break;
    case 0xf6: ori(); break;
    case 0xf7: rst(6); inc_pc = false; break;
    case 0xf8: inc_pc = ret(cpu->get_flag(Flag::S)); break;
    case 0xf9: sphl(); break;
    case 0xfa: inc_pc = jmp(cpu->get_flag(Flag::S)); break;
    case 0xfb: ei(); break;
    case 0xfc: inc_pc = call(cpu->get_flag(Flag::S)); break;
    case 0xfd: inc_pc = call(true); break;
    case 0xfe: cpi(); break;
    case 0xff: rst(7); inc_pc = false; break;

    default: std::cerr << "Unknown opcode\n"; break;
  }

  if (inc_pc)
    cpu->increment_pc(op.length);
}

void Emulator::nop() {}

void Emulator::lxi(Register x, Register y)
{
  cpu->set_register_pair(x, y, cpu->get_imm16());
}

void Emulator::lxi(Register x)
{
  if (x == Register::SP)
    cpu->set_sp(cpu->get_imm16());
}

void Emulator::stax(Register x, Register y)
{
  uint16_t addr = cpu->get_register_pair(x, y);
  cpu->store(addr, cpu->get_register(Register::A));
}

void Emulator::inx(Register x, Register y)
{
  uint16_t value = cpu->get_register_pair(x, y);
  cpu->set_register_pair(x, y, ++value);
}

void Emulator::inx(Register x)
{
  if (x == Register::SP)
    cpu->increment_sp(1);
}

void Emulator::inr(Register x)
{
  uint8_t before, after;

  if (x == Register::M)
  {
    uint16_t addr = cpu->get_register_pair(Register::H, Register::L);
    before = cpu->load(addr);
    after = before + 1;
    cpu->store(addr, after);
    cpu->affect_flags_szap(before, after);
    return;
  }

  before = cpu->get_register(x);
  after = before + 1;
  cpu->set_register(x, cpu->get_register(x) + 1);
  cpu->affect_flags_szap(before, after);
}

void Emulator::dcr(Register x)
{
  uint8_t before, after;

  if (x == Register::M)
  {
    uint16_t addr = cpu->get_register_pair(Register::H, Register::L);
    before = cpu->load(addr);
    after = before - 1;
    cpu->store(addr, after);
    cpu->affect_flags_szap(before, after);
    return;
  }

  before = cpu->get_register(x);
  after = before - 1;
  cpu->set_register(x, after);
  cpu->affect_flags_szap(before, after);
}

void Emulator::mvi(Register x)
{
  if (x == Register::M)
  {
    uint16_t addr = cpu->get_register_pair(Register::H, Register::L);
    cpu->store(addr, cpu->get_imm8());
    return;
  }

  cpu->set_register(x, cpu->get_imm8());
}

void Emulator::rlc(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t hb = (a & 0x80) >> 7;
  uint8_t res = (a << 1) | hb;
  cpu->set_register(Register::A, res);
  cpu->set_flag(Flag::C, (bool) hb);
}

void Emulator::dad(
  Register dst_x, Register dst_y, Register src_x, Register src_y)
{
  uint16_t hl = cpu->get_register_pair(dst_x, dst_y);
  uint16_t xy = cpu->get_register_pair(src_x, src_y);
  uint16_t res = hl + xy;

  cpu->set_register_pair(dst_x, dst_y, res);
  cpu->set_flag(Flag::C, res < hl || res < xy);
}

void Emulator::dad(
  Register dst_x, Register dst_y, Register src_x)
{
  if (src_x == Register::SP)
  {
    uint16_t hl = cpu->get_register_pair(dst_x, dst_y);
    uint16_t sp = cpu->get_sp();
    uint16_t res = hl + sp;
    cpu->set_register_pair(dst_x, dst_y, res);
    cpu->set_flag(Flag::C, res < hl || res < sp);
  }
}

void Emulator::ldax(Register x, Register y)
{
  uint16_t addr = cpu->get_register_pair(x, y);
  cpu->set_register(Register::A, cpu->load(addr));
}

void Emulator::dcx(Register x, Register y)
{
  uint16_t value = cpu->get_register_pair(x, y);
  cpu->set_register_pair(x, y, --value);
}

void Emulator::dcx(Register x)
{
  if (x == Register::SP)
    cpu->decrement_sp(1);
}

void Emulator::rrc(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t lb = a & 1;
  uint8_t res = (a >> 1) | (lb << 7);
  cpu->set_register(Register::A, res);
  cpu->set_flag(Flag::C, (bool) lb);
}

void Emulator::ral(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t hb = (a & 0x80) >> 7;
  uint8_t carry = cpu->get_flag(Flag::C) ? 1 : 0;
  uint8_t res = (a << 1) | carry;
  cpu->set_register(Register::A, res);
  cpu->set_flag(Flag::C, (bool) hb);
}

void Emulator::rar(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t carry = cpu->get_flag(Flag::C) ? 1 : 0;
  uint8_t lb = a & 1;
  uint8_t res = (a >> 1) | (carry << 7);
  cpu->set_register(Register::A, res); 
  cpu->set_flag(Flag::C, (bool) lb);
}

void Emulator::shld(void)
{
  uint16_t addr = cpu->get_imm16();
  cpu->store(addr, cpu->get_register(Register::L));
  cpu->store(addr+1, cpu->get_register(Register::H));
}

void Emulator::lhld(void)
{
  uint16_t addr = cpu->get_imm16();
  cpu->set_register(Register::L, cpu->load(addr));
  cpu->set_register(Register::H, cpu->load(addr+1));
}

void Emulator::cma(void)
{
  cpu->set_register(Register::A, ~cpu->get_register(Register::A));
}

void Emulator::sta(void)
{
  uint16_t addr = cpu->get_imm16();
  cpu->store(addr, cpu->get_register(Register::A));
}

void Emulator::lda(void)
{
  uint16_t addr = cpu->get_imm16();
  cpu->set_register(Register::A, cpu->load(addr));
}

void Emulator::mov(Register dst, Register src)
{
  cpu->set_register(dst, cpu->get_register(src));
}

void Emulator::add(Register src)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t val = cpu->get_register(src);
  uint8_t res = a + val;
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res < a);
}

void Emulator::adc(Register src)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t val = cpu->get_register(src);
  uint8_t carry = cpu->get_flag(Flag::C) ? 1 : 0;
  uint8_t res = a + val + carry;
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res < a);
}

void Emulator::sub(Register src)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t val = cpu->get_register(src);
  uint8_t res = a - val;
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res > a);
}

void Emulator::sbb(Register src)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t val = cpu->get_register(src);
  uint8_t carry = cpu->get_flag(Flag::C) ? 1 : 0;
  uint8_t res = a - val - carry;
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res > a);
}

void Emulator::ana(Register src)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t val = cpu->get_register(src);
  uint8_t res = a & val;
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, false);
}

void Emulator::xra(Register src)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t val = cpu->get_register(src);
  uint8_t res = a ^ val;
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, false);
}

void Emulator::ora(Register src)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t val = cpu->get_register(src);
  uint8_t res = a | val;
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, false);
}

void Emulator::cmp(Register src)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t val = cpu->get_register(src);
  uint8_t res = a - val;
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res > a);
}

void Emulator::cmc(void)
{
  cpu->set_flag(Flag::C, !cpu->get_flag(Flag::C));
}

void Emulator::stc(void)
{
  cpu->set_flag(Flag::C, true);
}

bool Emulator::call(bool condition)
{
  if (condition)
  {
    cpu->push(cpu->get_pc() + 3);
    cpu->set_pc(cpu->get_imm16());
    return false;
  }

  return true;
}

bool Emulator::ret(bool condition)
{
  if (condition)
  {
    uint16_t addr = cpu->pop();
    cpu->set_pc(addr);
    return false;
  }

  return true;
}

bool Emulator::jmp(bool condition)
{
  if (condition)
  {
    cpu->set_pc(cpu->get_imm16());
    return false;
  }

  return true;
}

void Emulator::rst(uint8_t n)
{
  cpu->push(cpu->get_pc() + 1);
  cpu->set_pc(n*8);
}

void Emulator::aci(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t res = a + cpu->get_imm8() + (uint8_t) cpu->get_flag(Flag::C);
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res < a);
}

void Emulator::adi(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t res = a + cpu->get_imm8();
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res < a);
}

void Emulator::push(Register x, Register y)
{
  cpu->store(cpu->get_sp() - 1, cpu->get_register(x));
  cpu->store(cpu->get_sp() - 2, cpu->get_register(y));
  cpu->decrement_sp(2);
}

void Emulator::pop(Register x, Register y)
{
  cpu->set_register(y, cpu->load(cpu->get_sp()));
  cpu->set_register(x, cpu->load(cpu->get_sp() + 1));
  cpu->increment_sp(2);
}

void Emulator::sui(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t res = a - cpu->get_imm8();
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res > a);
}

void Emulator::sbi(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t res = a - cpu->get_imm8() - (uint8_t) cpu->get_flag(Flag::C);
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res > a);
}

void Emulator::xthl(void)
{
  uint8_t h = cpu->get_register(Register::H);
  uint8_t l = cpu->get_register(Register::L);
  uint8_t sp_h = cpu->load(cpu->get_sp() + 1);
  uint8_t sp_l = cpu->load(cpu->get_sp());

  cpu->set_register(Register::H, sp_h);
  cpu->set_register(Register::L, sp_l);
  cpu->store(cpu->get_sp() + 1, h);
  cpu->store(cpu->get_sp(), l);
}

void Emulator::ani(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t res = a & cpu->get_imm8();
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res > a);
}

void Emulator::pchl(void)
{
  cpu->set_pc(cpu->get_register_pair(Register::H, Register::L));
}

void Emulator::xri(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t res = a ^ cpu->get_imm8();
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res > a);
}

void Emulator::xchg(void)
{
  uint16_t hl = cpu->get_register_pair(Register::H, Register::L);
  uint16_t de = cpu->get_register_pair(Register::D, Register::E);
  cpu->set_register_pair(Register::H, Register::L, de);
  cpu->set_register_pair(Register::D, Register::E, hl);
}

void Emulator::ori(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t res = a | cpu->get_imm8();
  cpu->set_register(Register::A, res);
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res > a);
}

void Emulator::sphl(void)
{
  cpu->set_sp(cpu->get_register_pair(Register::H, Register::L));
}

void Emulator::cpi(void)
{
  uint8_t a = cpu->get_register(Register::A);
  uint8_t res = a - cpu->get_imm8();
  cpu->affect_flags_szap(a, res);
  cpu->set_flag(Flag::C, res > a);
}

void Emulator::out(void)
{
  std::cout << cpu->get_register(Register::A);
}

void Emulator::in(void)
{
  char c = std::cin.get();
  cpu->set_register(Register::A, static_cast<uint8_t>(c));
}

void Emulator::ei(void)
{
  cpu->enable_interrupts();
}

void Emulator::di(void)
{
  cpu->disable_interrupts();
}

void Emulator::daa(void)
{
  uint8_t a = cpu->get_register(Register::A);

  uint8_t lb = a & 0x0f;
  bool lb_condition = (lb > 9) || cpu->get_flag(Flag::AC);
  uint8_t lb_adj = lb_condition ? (a + 6) : a;

  uint8_t hb = (lb_adj & 0xf0) >> 4;
  bool hb_condition = (hb > 9) || cpu->get_flag(Flag::C);
  uint8_t hb_adj;
  if (hb_condition)
  {
    hb_adj = ((hb + 6) << 4) | (lb_adj & 0x0f);
  }
  else
  {
    hb_adj = lb_adj;
  }

  bool carry = ((hb_adj & 0xf0) >> 4) < hb;

  cpu->set_register(Register::A, hb_adj);
  cpu->affect_flags_szap(a, hb_adj);
  cpu->set_flag(Flag::C, carry);
}
