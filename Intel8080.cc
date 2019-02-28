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
  opcodes.push_back(Opcode(0x04, "inr", "Increment register B", &Intel8080::op_inr<Register::B>));
  opcodes.push_back(Opcode(0x05, "dcr", "Decrement register B", &Intel8080::op_dcr<Register::B>));
  opcodes.push_back(Opcode(0x06, "mvi", "Move immediate to B", &Intel8080::op_mvi<Register::B>));
  opcodes.push_back(Opcode(0x07, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x08, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x09, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x0c, "inr", "Increment register C", &Intel8080::op_inr<Register::C>));
  opcodes.push_back(Opcode(0x0d, "dcr", "Decrement register C", &Intel8080::op_dcr<Register::C>));
  opcodes.push_back(Opcode(0x0e, "mvi", "Move immediate to C", &Intel8080::op_mvi<Register::C>));
  opcodes.push_back(Opcode(0x0f, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x10, "dump", "Print registers", &Intel8080::op_dump));
  opcodes.push_back(Opcode(0x11, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x12, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x13, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x14, "inr", "Increment register D", &Intel8080::op_inr<Register::D>));
  opcodes.push_back(Opcode(0x15, "dcr", "Unknown instruction", &Intel8080::op_dcr<Register::D>));
  opcodes.push_back(Opcode(0x16, "mvi", "Move immediate to D", &Intel8080::op_mvi<Register::D>));
  opcodes.push_back(Opcode(0x17, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x18, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x19, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x1c, "inr", "Increment register E", &Intel8080::op_inr<Register::E>));
  opcodes.push_back(Opcode(0x1d, "dcr", "Decrement register E", &Intel8080::op_dcr<Register::E>));
  opcodes.push_back(Opcode(0x1e, "mvi", "Move immediate to E", &Intel8080::op_mvi<Register::E>));
  opcodes.push_back(Opcode(0x1f, "null", "Unknown instruction", nullptr));
  
  opcodes.push_back(Opcode(0x20, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x21, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x22, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x23, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x24, "inr", "Increment register H", &Intel8080::op_inr<Register::H>));
  opcodes.push_back(Opcode(0x25, "dcr", "Decrement register H", &Intel8080::op_dcr<Register::H>));
  opcodes.push_back(Opcode(0x26, "mvi", "Move immediate to H", &Intel8080::op_mvi<Register::H>));
  opcodes.push_back(Opcode(0x27, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x28, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x29, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x2c, "inr", "Increment register L", &Intel8080::op_inr<Register::L>));
  opcodes.push_back(Opcode(0x2d, "dcr", "Decrement register L", &Intel8080::op_dcr<Register::L>));
  opcodes.push_back(Opcode(0x2e, "mvi", "Move immediate to L", &Intel8080::op_mvi<Register::L>));
  opcodes.push_back(Opcode(0x2f, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x30, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x31, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x32, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x33, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x34, "inr", "Increment memref at H:L", &Intel8080::op_inr<Register::M>));
  opcodes.push_back(Opcode(0x35, "dcr", "Decrement memref at H:L", &Intel8080::op_dcr<Register::M>));
  opcodes.push_back(Opcode(0x36, "mvi", "Move immediate to memref at H:L", &Intel8080::op_mvi<Register::M>));
  opcodes.push_back(Opcode(0x37, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x38, "nop", "No operation", &Intel8080::op_nop));
  opcodes.push_back(Opcode(0x39, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3a, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3b, "null", "Unknown instruction", nullptr));
  opcodes.push_back(Opcode(0x3c, "inr", "Increment register A", &Intel8080::op_inr<Register::A>));
  opcodes.push_back(Opcode(0x3d, "dcr", "Decrement register A", &Intel8080::op_dcr<Register::A>));
  opcodes.push_back(Opcode(0x3e, "mvi", "Move immediate to A", &Intel8080::op_mvi<Register::A>));
  opcodes.push_back(Opcode(0x3f, "null", "Unknown instruction", nullptr));

  opcodes.push_back(Opcode(0x40, "mov", "Move B to B", &Intel8080::op_mov<Register::B, Register::B>));
  opcodes.push_back(Opcode(0x41, "mov", "Move C to B", &Intel8080::op_mov<Register::B, Register::C>));
  opcodes.push_back(Opcode(0x42, "mov", "Move D to B", &Intel8080::op_mov<Register::B, Register::D>));
  opcodes.push_back(Opcode(0x43, "mov", "Move E to B", &Intel8080::op_mov<Register::B, Register::E>));
  opcodes.push_back(Opcode(0x44, "mov", "Move H to B", &Intel8080::op_mov<Register::B, Register::H>));
  opcodes.push_back(Opcode(0x45, "mov", "Move L to B", &Intel8080::op_mov<Register::B, Register::L>));
  opcodes.push_back(Opcode(0x46, "mov", "Move M to B", &Intel8080::op_mov<Register::B, Register::M>));
  opcodes.push_back(Opcode(0x47, "mov", "Move A to B", &Intel8080::op_mov<Register::B, Register::A>));
  opcodes.push_back(Opcode(0x48, "mov", "Move B to C", &Intel8080::op_mov<Register::C, Register::B>));
  opcodes.push_back(Opcode(0x49, "mov", "Move C to C", &Intel8080::op_mov<Register::C, Register::C>));
  opcodes.push_back(Opcode(0x4a, "mov", "Move D to C", &Intel8080::op_mov<Register::C, Register::D>));
  opcodes.push_back(Opcode(0x4b, "mov", "Move E to C", &Intel8080::op_mov<Register::C, Register::E>));
  opcodes.push_back(Opcode(0x4c, "mov", "Move H to C", &Intel8080::op_mov<Register::C, Register::H>));
  opcodes.push_back(Opcode(0x4d, "mov", "Move L to C", &Intel8080::op_mov<Register::C, Register::L>));
  opcodes.push_back(Opcode(0x4e, "mov", "Move M to C", &Intel8080::op_mov<Register::C, Register::M>));
  opcodes.push_back(Opcode(0x4f, "mov", "Move A to C", &Intel8080::op_mov<Register::C, Register::A>));

  opcodes.push_back(Opcode(0x50, "mov", "Move B to D", &Intel8080::op_mov<Register::D, Register::B>));
  opcodes.push_back(Opcode(0x51, "mov", "Move C to D", &Intel8080::op_mov<Register::D, Register::C>));
  opcodes.push_back(Opcode(0x52, "mov", "Move D to D", &Intel8080::op_mov<Register::D, Register::D>));
  opcodes.push_back(Opcode(0x53, "mov", "Move E to D", &Intel8080::op_mov<Register::D, Register::E>));
  opcodes.push_back(Opcode(0x54, "mov", "Move H to D", &Intel8080::op_mov<Register::D, Register::H>));
  opcodes.push_back(Opcode(0x55, "mov", "Move L to D", &Intel8080::op_mov<Register::D, Register::L>));
  opcodes.push_back(Opcode(0x56, "mov", "Move M to D", &Intel8080::op_mov<Register::D, Register::M>));
  opcodes.push_back(Opcode(0x57, "mov", "Move A to D", &Intel8080::op_mov<Register::D, Register::A>));
  opcodes.push_back(Opcode(0x58, "mov", "Move B to E", &Intel8080::op_mov<Register::E, Register::B>));
  opcodes.push_back(Opcode(0x59, "mov", "Move C to E", &Intel8080::op_mov<Register::E, Register::C>));
  opcodes.push_back(Opcode(0x5a, "mov", "Move D to E", &Intel8080::op_mov<Register::E, Register::D>));
  opcodes.push_back(Opcode(0x5b, "mov", "Move E to E", &Intel8080::op_mov<Register::E, Register::E>));
  opcodes.push_back(Opcode(0x5c, "mov", "Move H to E", &Intel8080::op_mov<Register::E, Register::H>));
  opcodes.push_back(Opcode(0x5d, "mov", "Move L to E", &Intel8080::op_mov<Register::E, Register::L>));
  opcodes.push_back(Opcode(0x5e, "mov", "Move M to E", &Intel8080::op_mov<Register::E, Register::M>));
  opcodes.push_back(Opcode(0x5f, "mov", "Move A to E", &Intel8080::op_mov<Register::E, Register::A>));

  opcodes.push_back(Opcode(0x60, "mov", "Move B to H", &Intel8080::op_mov<Register::H, Register::B>));
  opcodes.push_back(Opcode(0x61, "mov", "Move C to H", &Intel8080::op_mov<Register::H, Register::C>));
  opcodes.push_back(Opcode(0x62, "mov", "Move D to H", &Intel8080::op_mov<Register::H, Register::D>));
  opcodes.push_back(Opcode(0x63, "mov", "Move E to H", &Intel8080::op_mov<Register::H, Register::E>));
  opcodes.push_back(Opcode(0x64, "mov", "Move H to H", &Intel8080::op_mov<Register::H, Register::H>));
  opcodes.push_back(Opcode(0x65, "mov", "Move L to H", &Intel8080::op_mov<Register::H, Register::L>));
  opcodes.push_back(Opcode(0x66, "mov", "Move M to H", &Intel8080::op_mov<Register::H, Register::M>));
  opcodes.push_back(Opcode(0x67, "mov", "Move A to H", &Intel8080::op_mov<Register::H, Register::A>));
  opcodes.push_back(Opcode(0x68, "mov", "Move B to L", &Intel8080::op_mov<Register::L, Register::B>));
  opcodes.push_back(Opcode(0x69, "mov", "Move C to L", &Intel8080::op_mov<Register::L, Register::C>));
  opcodes.push_back(Opcode(0x6a, "mov", "Move D to L", &Intel8080::op_mov<Register::L, Register::D>));
  opcodes.push_back(Opcode(0x6b, "mov", "Move E to L", &Intel8080::op_mov<Register::L, Register::E>));
  opcodes.push_back(Opcode(0x6c, "mov", "Move H to L", &Intel8080::op_mov<Register::L, Register::H>));
  opcodes.push_back(Opcode(0x6d, "mov", "Move L to L", &Intel8080::op_mov<Register::L, Register::L>));
  opcodes.push_back(Opcode(0x6e, "mov", "Move M to L", &Intel8080::op_mov<Register::L, Register::M>));
  opcodes.push_back(Opcode(0x6f, "mov", "Move A to L", &Intel8080::op_mov<Register::L, Register::A>));

  opcodes.push_back(Opcode(0x70, "mov", "Move B to M", &Intel8080::op_mov<Register::M, Register::B>));
  opcodes.push_back(Opcode(0x71, "mov", "Move C to M", &Intel8080::op_mov<Register::M, Register::C>));
  opcodes.push_back(Opcode(0x72, "mov", "Move D to M", &Intel8080::op_mov<Register::M, Register::D>));
  opcodes.push_back(Opcode(0x73, "mov", "Move E to M", &Intel8080::op_mov<Register::M, Register::E>));
  opcodes.push_back(Opcode(0x74, "mov", "Move H to M", &Intel8080::op_mov<Register::M, Register::H>));
  opcodes.push_back(Opcode(0x75, "mov", "Move L to M", &Intel8080::op_mov<Register::M, Register::L>));
  opcodes.push_back(Opcode(0x76, "hlt", "Halt processor (unimplemented)", nullptr));
  opcodes.push_back(Opcode(0x77, "mov", "Move A to M", &Intel8080::op_mov<Register::M, Register::A>));
  opcodes.push_back(Opcode(0x78, "mov", "Move B to A", &Intel8080::op_mov<Register::A, Register::B>));
  opcodes.push_back(Opcode(0x79, "mov", "Move C to A", &Intel8080::op_mov<Register::A, Register::C>));
  opcodes.push_back(Opcode(0x7a, "mov", "Move D to A", &Intel8080::op_mov<Register::A, Register::D>));
  opcodes.push_back(Opcode(0x7b, "mov", "Move E to A", &Intel8080::op_mov<Register::A, Register::E>));
  opcodes.push_back(Opcode(0x7c, "mov", "Move H to A", &Intel8080::op_mov<Register::A, Register::H>));
  opcodes.push_back(Opcode(0x7d, "mov", "Move L to A", &Intel8080::op_mov<Register::A, Register::L>));
  opcodes.push_back(Opcode(0x7e, "mov", "Move M to A", &Intel8080::op_mov<Register::A, Register::M>));
  opcodes.push_back(Opcode(0x7f, "mov", "Move A to A", &Intel8080::op_mov<Register::A, Register::A>));

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
  printf("\n==============REGISTERS==============\n");
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
  printf("=====================================\n\n");
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

template <Intel8080::Register reg>
void Intel8080::op_inr()
{
  uint8_t val = getRegisterValue(reg);
  setRegisterValue(reg, val + 1);
  // TODO: set flags register
  pc++;
}

template <Intel8080::Register reg>
void Intel8080::op_dcr()
{
  uint8_t val = getRegisterValue(reg);
  setRegisterValue(reg, val - 1);
  // TODO: set flags register
  pc++;
}

template <Intel8080::Register reg>
void Intel8080::op_mvi()
{
  uint8_t val = memory[pc+1];
  setRegisterValue(reg, val);
  pc += 2;
}

template <Intel8080::Register destination, Intel8080::Register source>
void Intel8080::op_mov()
{
  setRegisterValue(destination, getRegisterValue(source));
  pc++;
}
