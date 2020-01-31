#ifndef OPCODE_H
#define OPCODE_H

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>

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

std::vector<Opcode> load_opcodes(std::string filename);

#endif
