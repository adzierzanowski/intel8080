#include "Intel8080.h"

Opcode::Opcode(const uint8_t number,
  const std::string mnemonic,
  const std::string fullName,
  func_t callback):
    number(number), mnemonic(mnemonic), fullName(fullName), callback(callback)
{
}

Intel8080::Intel8080()
{
  memory.resize(0x10000);
  generateOpcodes();
}

void Intel8080::generateOpcodes()
{
  opcodes.push_back(Opcode(0x00, "term", "Terminate program", &Intel8080::op_term));
  opcodes.push_back(Opcode(0x01, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x02, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x03, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x04, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x05, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x06, "mvi", "Move immediate to B", &Intel8080::op_mvi_b));
  opcodes.push_back(Opcode(0x07, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x08, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x09, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0c, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0d, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0e, "mvi", "Move immediate to C", &Intel8080::op_mvi_c));
  opcodes.push_back(Opcode(0x0f, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x10, "dump", "Print registers", &Intel8080::op_dump));
  opcodes.push_back(Opcode(0x11, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x12, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x13, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x14, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x15, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x16, "mvi", "Move immediate to D", &Intel8080::op_mvi_d));
  opcodes.push_back(Opcode(0x17, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x18, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x19, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1c, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1d, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1e, "mvi", "Move immediate to E", &Intel8080::op_mvi_e));
  opcodes.push_back(Opcode(0x1f, "null", "Unknown instruction", nullptr));
  
  opcodes.push_back(Opcode(0x20, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x21, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x22, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x23, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x24, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x25, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x26, "mvi", "Move immediate to H", &Intel8080::op_mvi_h));
  opcodes.push_back(Opcode(0x27, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x28, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x29, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2c, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2d, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2e, "mvi", "Move immediate to L", &Intel8080::op_mvi_l));
  opcodes.push_back(Opcode(0x2f, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x30, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x31, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x32, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x33, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x34, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x35, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x36, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x37, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x38, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x39, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3c, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3d, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3e, "mvi", "Move immediate to A", &Intel8080::op_mvi_a));
  opcodes.push_back(Opcode(0x3f, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x40, "mov", "Move B to B", &Intel8080::op_mov_b_b));
  opcodes.push_back(Opcode(0x41, "mov", "Move C to B", &Intel8080::op_mov_b_c));
  opcodes.push_back(Opcode(0x42, "mov", "Move D to B", &Intel8080::op_mov_b_d));
  opcodes.push_back(Opcode(0x43, "mov", "Move E to B", &Intel8080::op_mov_b_e));
  opcodes.push_back(Opcode(0x44, "mov", "Move H to B", &Intel8080::op_mov_b_h));
  opcodes.push_back(Opcode(0x45, "mov", "Move L to B", &Intel8080::op_mov_b_l));
  opcodes.push_back(Opcode(0x46, "mov", "Move M to B", &Intel8080::op_mov_b_m));
  opcodes.push_back(Opcode(0x47, "mov", "Move A to B", &Intel8080::op_mov_b_a));
  opcodes.push_back(Opcode(0x48, "mov", "Move B to C", &Intel8080::op_mov_c_b));
  opcodes.push_back(Opcode(0x49, "mov", "Move C to C", &Intel8080::op_mov_c_c));
  opcodes.push_back(Opcode(0x4a, "mov", "Move D to C", &Intel8080::op_mov_c_d));
  opcodes.push_back(Opcode(0x4b, "mov", "Move E to C", &Intel8080::op_mov_c_e));
  opcodes.push_back(Opcode(0x4c, "mov", "Move H to C", &Intel8080::op_mov_c_h));
  opcodes.push_back(Opcode(0x4d, "mov", "Move L to C", &Intel8080::op_mov_c_l));
  opcodes.push_back(Opcode(0x4e, "mov", "Move M to C", &Intel8080::op_mov_c_m));
  opcodes.push_back(Opcode(0x4f, "mov", "Move A to C", &Intel8080::op_mov_c_a));

  opcodes.push_back(Opcode(0x50, "mov", "Move B to D", nullptr));
  opcodes.push_back(Opcode(0x51, "mov", "Move C to D", nullptr));
  opcodes.push_back(Opcode(0x52, "mov", "Move D to D", nullptr));
  opcodes.push_back(Opcode(0x53, "mov", "Move E to D", nullptr));
  opcodes.push_back(Opcode(0x54, "mov", "Move H to D", nullptr));
  opcodes.push_back(Opcode(0x55, "mov", "Move L to D", nullptr));
  opcodes.push_back(Opcode(0x56, "mov", "Move M to D", nullptr));
  opcodes.push_back(Opcode(0x57, "mov", "Move A to D", nullptr));
  opcodes.push_back(Opcode(0x58, "mov", "Move B to E", nullptr));
  opcodes.push_back(Opcode(0x59, "mov", "Move C to E", nullptr));
  opcodes.push_back(Opcode(0x5a, "mov", "Move D to E", nullptr));
  opcodes.push_back(Opcode(0x5b, "mov", "Move E to E", nullptr));
  opcodes.push_back(Opcode(0x5c, "mov", "Move H to E", nullptr));
  opcodes.push_back(Opcode(0x5d, "mov", "Move L to E", nullptr));
  opcodes.push_back(Opcode(0x5e, "mov", "Move M to E", nullptr));
  opcodes.push_back(Opcode(0x5f, "mov", "Move A to E", nullptr));

  opcodes.push_back(Opcode(0x60, "mov", "Move B to H", nullptr));
  opcodes.push_back(Opcode(0x61, "mov", "Move C to H", nullptr));
  opcodes.push_back(Opcode(0x62, "mov", "Move D to H", nullptr));
  opcodes.push_back(Opcode(0x63, "mov", "Move E to H", nullptr));
  opcodes.push_back(Opcode(0x64, "mov", "Move H to H", nullptr));
  opcodes.push_back(Opcode(0x65, "mov", "Move L to H", nullptr));
  opcodes.push_back(Opcode(0x66, "mov", "Move M to H", nullptr));
  opcodes.push_back(Opcode(0x67, "mov", "Move A to H", nullptr));
  opcodes.push_back(Opcode(0x68, "mov", "Move B to L", nullptr));
  opcodes.push_back(Opcode(0x69, "mov", "Move C to L", nullptr));
  opcodes.push_back(Opcode(0x6a, "mov", "Move D to L", nullptr));
  opcodes.push_back(Opcode(0x6b, "mov", "Move E to L", nullptr));
  opcodes.push_back(Opcode(0x6c, "mov", "Move H to L", nullptr));
  opcodes.push_back(Opcode(0x6d, "mov", "Move L to L", nullptr));
  opcodes.push_back(Opcode(0x6e, "mov", "Move M to L", nullptr));
  opcodes.push_back(Opcode(0x6f, "mov", "Move A to L", nullptr));

  opcodes.push_back(Opcode(0x70, "mov", "Move B to M", nullptr));
  opcodes.push_back(Opcode(0x71, "mov", "Move C to M", nullptr));
  opcodes.push_back(Opcode(0x72, "mov", "Move D to M", nullptr));
  opcodes.push_back(Opcode(0x73, "mov", "Move E to M", nullptr));
  opcodes.push_back(Opcode(0x74, "mov", "Move H to M", nullptr));
  opcodes.push_back(Opcode(0x75, "mov", "Move M to M", nullptr));
  opcodes.push_back(Opcode(0x76, "hlt", "Halt processor (unimplemented)", nullptr));
  opcodes.push_back(Opcode(0x77, "mov", "Move A to M", nullptr));
  opcodes.push_back(Opcode(0x78, "mov", "Move B to A", nullptr));
  opcodes.push_back(Opcode(0x79, "mov", "Move C to A", nullptr));
  opcodes.push_back(Opcode(0x7a, "mov", "Move D to A", nullptr));
  opcodes.push_back(Opcode(0x7b, "mov", "Move E to A", nullptr));
  opcodes.push_back(Opcode(0x7c, "mov", "Move H to A", nullptr));
  opcodes.push_back(Opcode(0x7d, "mov", "Move L to A", nullptr));
  opcodes.push_back(Opcode(0x7e, "mov", "Move M to A", nullptr));
  opcodes.push_back(Opcode(0x7f, "mov", "Move A to A", nullptr));

  opcodes.push_back(Opcode(0x80, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x81, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x82, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x83, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x84, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x85, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x86, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x87, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x88, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x89, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8c, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8d, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8e, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x8f, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x90, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x91, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x92, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x93, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x94, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x95, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x96, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x97, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x98, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x99, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9c, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9d, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9e, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x9f, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xa0, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa2, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa3, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa4, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa5, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa6, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa7, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa8, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xa9, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xaa, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xab, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xac, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xad, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xae, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xaf, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xb0, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb2, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb3, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb4, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb5, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb6, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb7, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb8, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xb9, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xba, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbb, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbc, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbd, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbe, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xbf, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xc0, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc2, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc3, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc4, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc5, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc6, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc7, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc8, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xc9, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xca, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xcb, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xcc, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xcd, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xce, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xcf, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xd0, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd2, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd3, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd4, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd5, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd6, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd7, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd8, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xd9, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xda, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xdb, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xdc, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xdd, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xde, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xdf, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xe0, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe2, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe3, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe4, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe5, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe6, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe7, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe8, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xe9, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xea, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xeb, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xec, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xed, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xee, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xef, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0xf0, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf1, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf2, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf3, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf4, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf5, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf6, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf7, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf8, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xf9, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xfa, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xfb, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xfc, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xfd, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xfe, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0xff, "null", "Unknown instruction", nullptr));
}

void Intel8080::executeInstruction(uint8_t opcode_n)
{
  Opcode opcode = opcodes[opcode_n];
  printf(
    "\033[1m0x%02x\033[0m: \033[38;5;13m%s\033[0m %s\n",
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
  printf("\n===REGISTERS===\n");
  printf("\033[1ma\033[0m: 0x%02x | ", a);
  printf("\033[1mb\033[0m: 0x%02x | ", b);
  printf("\033[1mc\033[0m: 0x%02x | ", c);
  printf("\033[1md\033[0m: 0x%02x\n", d);
  printf("\033[1me\033[0m: 0x%02x | ", e);
  printf("\033[1mh\033[0m: 0x%02x | ", h);
  printf("\033[1ml\033[0m: 0x%02x\n", l);

  printf("\033[1msp\033[0m: 0x%02x | ", sp);
  printf("\033[1mpc\033[0m: 0x%02x | ", pc);
  printf("\033[1mflags\033[0m: 0x%02x\n", flags);
  printf("===============\n\n");
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

void Intel8080::execute()
{
  uint8_t opcode_n = memory[pc];
  executeInstruction(opcode_n);
  if (!terminateFlag)
    execute();
}

void Intel8080::setProgramCounter(uint16_t address)
{
  pc = address;
}

void Intel8080::op_dump()
{
  printRegisters();
  pc++;
}

void Intel8080::op_term()
{
  terminateFlag = true;
}

void Intel8080::op_nop()
{
  pc++;
}

void Intel8080::op_mvi(Intel8080::Register reg)
{
  uint8_t val = memory[pc+1];
  setRegisterValue(reg, val);
  pc += 2;
}

void Intel8080::op_mvi_a() { op_mvi(Register::A); }
void Intel8080::op_mvi_b() { op_mvi(Register::B); }
void Intel8080::op_mvi_c() { op_mvi(Register::C); }
void Intel8080::op_mvi_d() { op_mvi(Register::D); }
void Intel8080::op_mvi_e() { op_mvi(Register::E); }
void Intel8080::op_mvi_l() { op_mvi(Register::L); }
void Intel8080::op_mvi_h() { op_mvi(Register::H); }

void Intel8080::op_mov(Intel8080::Register destination, Intel8080::Register source)
{
  setRegisterValue(destination, getRegisterValue(source));
  pc++;
}

void Intel8080::op_mov_b_b() { op_mov(Register::B, Register::B); }
void Intel8080::op_mov_b_c() { op_mov(Register::B, Register::C); }
void Intel8080::op_mov_b_d() { op_mov(Register::B, Register::D); }
void Intel8080::op_mov_b_e() { op_mov(Register::B, Register::E); }
void Intel8080::op_mov_b_h() { op_mov(Register::B, Register::H); }
void Intel8080::op_mov_b_l() { op_mov(Register::B, Register::L); }
void Intel8080::op_mov_b_m() { op_mov(Register::B, Register::M); }
void Intel8080::op_mov_b_a() { op_mov(Register::B, Register::A); }

void Intel8080::op_mov_c_b() { op_mov(Register::C, Register::B); }
void Intel8080::op_mov_c_c() { op_mov(Register::C, Register::C); }
void Intel8080::op_mov_c_d() { op_mov(Register::C, Register::D); }
void Intel8080::op_mov_c_e() { op_mov(Register::C, Register::E); }
void Intel8080::op_mov_c_h() { op_mov(Register::C, Register::H); }
void Intel8080::op_mov_c_l() { op_mov(Register::C, Register::L); }
void Intel8080::op_mov_c_m() { op_mov(Register::C, Register::M); }
void Intel8080::op_mov_c_a() { op_mov(Register::C, Register::A); }

void Intel8080::op_mov_d_b() { op_mov(Register::D, Register::B); }
void Intel8080::op_mov_d_c() { op_mov(Register::D, Register::C); }
void Intel8080::op_mov_d_d() { op_mov(Register::D, Register::D); }
void Intel8080::op_mov_d_e() { op_mov(Register::D, Register::E); }
void Intel8080::op_mov_d_h() { op_mov(Register::D, Register::H); }
void Intel8080::op_mov_d_l() { op_mov(Register::D, Register::L); }
void Intel8080::op_mov_d_m() { op_mov(Register::D, Register::M); }
void Intel8080::op_mov_d_a() { op_mov(Register::D, Register::A); }

void Intel8080::op_mov_e_b() { op_mov(Register::E, Register::B); }
void Intel8080::op_mov_e_c() { op_mov(Register::E, Register::C); }
void Intel8080::op_mov_e_d() { op_mov(Register::E, Register::D); }
void Intel8080::op_mov_e_e() { op_mov(Register::E, Register::E); }
void Intel8080::op_mov_e_h() { op_mov(Register::E, Register::H); }
void Intel8080::op_mov_e_l() { op_mov(Register::E, Register::L); }
void Intel8080::op_mov_e_m() { op_mov(Register::E, Register::M); }
void Intel8080::op_mov_e_a() { op_mov(Register::E, Register::A); }

void Intel8080::op_mov_h_b() { op_mov(Register::H, Register::B); }
void Intel8080::op_mov_h_c() { op_mov(Register::H, Register::C); }
void Intel8080::op_mov_h_d() { op_mov(Register::H, Register::D); }
void Intel8080::op_mov_h_e() { op_mov(Register::H, Register::E); }
void Intel8080::op_mov_h_h() { op_mov(Register::H, Register::H); }
void Intel8080::op_mov_h_l() { op_mov(Register::H, Register::L); }
void Intel8080::op_mov_h_m() { op_mov(Register::H, Register::M); }
void Intel8080::op_mov_h_a() { op_mov(Register::H, Register::A); }

void Intel8080::op_mov_l_b() { op_mov(Register::L, Register::B); }
void Intel8080::op_mov_l_c() { op_mov(Register::L, Register::C); }
void Intel8080::op_mov_l_d() { op_mov(Register::L, Register::D); }
void Intel8080::op_mov_l_e() { op_mov(Register::L, Register::E); }
void Intel8080::op_mov_l_h() { op_mov(Register::L, Register::H); }
void Intel8080::op_mov_l_l() { op_mov(Register::L, Register::L); }
void Intel8080::op_mov_l_m() { op_mov(Register::L, Register::M); }
void Intel8080::op_mov_l_a() { op_mov(Register::L, Register::A); }

void Intel8080::op_mov_m_b() { op_mov(Register::M, Register::B); }
void Intel8080::op_mov_m_c() { op_mov(Register::M, Register::C); }
void Intel8080::op_mov_m_d() { op_mov(Register::M, Register::D); }
void Intel8080::op_mov_m_e() { op_mov(Register::M, Register::E); }
void Intel8080::op_mov_m_h() { op_mov(Register::M, Register::H); }
void Intel8080::op_mov_m_l() { op_mov(Register::M, Register::L); }
// no such thing as mov M, M
void Intel8080::op_mov_m_a() { op_mov(Register::M, Register::A); }

void Intel8080::op_mov_a_b() { op_mov(Register::A, Register::B); }
void Intel8080::op_mov_a_c() { op_mov(Register::A, Register::C); }
void Intel8080::op_mov_a_d() { op_mov(Register::A, Register::D); }
void Intel8080::op_mov_a_e() { op_mov(Register::A, Register::E); }
void Intel8080::op_mov_a_h() { op_mov(Register::A, Register::H); }
void Intel8080::op_mov_a_l() { op_mov(Register::A, Register::L); }
void Intel8080::op_mov_a_m() { op_mov(Register::A, Register::M); }
void Intel8080::op_mov_a_a() { op_mov(Register::A, Register::A); }
