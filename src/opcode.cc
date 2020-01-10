#include "opcode.hh"


Opcode::Opcode(
  uint8_t number,
  uint8_t length,
  uint8_t cycles,
  std::string mnemonic,
  std::string description
) :
  number{number},
  length{length},
  cycles{cycles},
  mnemonic{mnemonic},
  description{description}
{
}
