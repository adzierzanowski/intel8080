#ifndef OPCODE_H
#define OPCODE_H

#include <cstdint>
#include <string>


struct Opcode
{
  uint8_t number;
  uint8_t length;
  uint8_t cycles;
  std::string mnemonic;
  std::string description;

  Opcode(
    uint8_t number,
    uint8_t length,
    uint8_t cycles,
    std::string mnemonic,
    std::string description
  );
};

#endif
