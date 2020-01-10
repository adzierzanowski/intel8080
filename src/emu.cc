#include "emu.hh"


const std::string Emulator::opcodes_filename = "opcodes.csv";

Emulator::Emulator(void) :
  cpu{std::make_unique<CPU>()},
  opcodes{std::vector<Opcode>()},
  execute_flag{false}
{
  load_opcodes();
}

void Emulator::load_program(std::vector<uint8_t> prog, uint16_t address)
{
  cpu->load_program(prog, address);
}

void Emulator::load_opcodes(void)
{
  std::ifstream opfile(Emulator::opcodes_filename);
  if (opfile.fail())
  {
    std::cerr << "Failed to load opcodes\n";
    std::exit(1);
  }

  std::string line;
  std::getline(opfile, line); // Skip the header
  int i = 0;
  while (std::getline(opfile, line))
  {
    if (line != "")
    {
      std::vector<std::string> split_line;
      boost::split(split_line, line, boost::is_any_of(","));

      uint8_t op_number = i;
      std::string op_mnemonic = split_line[1];
      uint8_t op_length = std::stoul(split_line[2]);
      uint8_t op_cycles = std::stoul(split_line[3]);
      std::string op_description = split_line[4];

      boost::trim(op_mnemonic);
      boost::trim(op_description);

      Opcode op(op_number, op_length, op_cycles, op_mnemonic, op_description);
      opcodes.push_back(op);

      i++;
    }
  }
}

void Emulator::execute(void)
{
  execute_flag = true;
  uint8_t opcode = cpu->get_opcode();
  execute_opcode(opcode);

  if (execute_flag)
    execute();
}

void Emulator::execute_opcode(uint8_t opcode)
{
  auto op = opcodes[opcode];

  //std::cout << boost::format("%d %4s %-20s") % cpu->get_pc() % op.mnemonic % op.description << "\n";

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
    // TODO: case 0x27: daa(); break;
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
    // TODO: case 0x37: stc(); break;
    case 0x39: dad(Register::H, Register::L, Register::SP); break;
    case 0x3a: lda(); break;
    case 0x3b: dcx(Register::SP); break;
    case 0x3c: inr(Register::A); break;
    case 0x3d: dcr(Register::A); break;
    case 0x3e: mvi(Register::A); break;
    // TODO: case 0x3f: cmc(); break;

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

    default: std::cerr << "Unknown opcode\n"; break;
  }

  cpu->increase_program_counter(op.length);
}

void Emulator::nop() {}

void Emulator::lxi(Register x, Register y)
{
  cpu->set_register_pair(x, y, cpu->get_imm16());
}

void Emulator::lxi(Register x)
{
  if (x == Register::SP)
    cpu->set_stack_pointer(cpu->get_imm16());
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
    cpu->increase_stack_pointer(1);
}

void Emulator::inr(Register x)
{
  if (x == Register::M)
  {
    uint16_t addr = cpu->get_register_pair(Register::H, Register::L);
    cpu->store(addr, cpu->load(addr)+1);
    return;
  }

  cpu->set_register(x, cpu->get_register(x) + 1);
}

void Emulator::dcr(Register x)
{
  if (x == Register::M)
  {
    uint16_t addr = cpu->get_register_pair(Register::H, Register::L);
    cpu->store(addr, cpu->load(addr)-1);
    return;
  }

  cpu->set_register(x, cpu->get_register(x) - 1);
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
  cpu->set_register(Register::A, cpu->get_register(Register::A) << 1);
}

void Emulator::dad(
  Register dst_x, Register dst_y, Register src_x, Register src_y)
{
  uint16_t hl = cpu->get_register_pair(dst_x, dst_y);
  hl += cpu->get_register_pair(src_x, src_y);
  cpu->set_register_pair(dst_x, dst_y, hl);
}

void Emulator::dad(
  Register dst_x, Register dst_y, Register src_x)
{
  if (src_x == Register::SP)
  {
    uint16_t hl = cpu->get_register_pair(dst_x, dst_y);
    hl += cpu->get_stack_pointer();
    cpu->set_register_pair(dst_x, dst_y, hl);
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
    cpu->decrease_stack_pointer(1);
}

void Emulator::rrc(void)
{
  cpu->set_register(Register::A, cpu->get_register(Register::A) >> 1);
}

void Emulator::ral(void)
{
  cpu->set_register(Register::A, cpu->get_register(Register::A) << 1);
}

void Emulator::rar(void)
{
  cpu->set_register(Register::A, cpu->get_register(Register::A) >> 1);
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
  uint8_t sum = cpu->get_register(Register::A) + cpu->get_register(src);
  cpu->set_register(Register::A, sum);
}
