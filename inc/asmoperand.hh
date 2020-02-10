#ifndef ASMOPERAND_H
#define ASMOPERAND_H

#include "asmconstraint.hh"
#include "token.hh"


struct AsmOp
{
  uint8_t opcode_template;
  std::string mnemonic;
  std::vector<Token::Type> operands;
  std::vector<Constraint> constraints;
  std::vector<int> opcode_bit_pos;

  AsmOp() {}
  AsmOp(
    uint8_t opcode_template,
    std::initializer_list<Token::Type> operands,
    std::initializer_list<Constraint> constraints,
    std::initializer_list<int> opcode_bit_pos
  )
    : opcode_template{opcode_template},
      operands{operands},
      constraints{constraints},
      opcode_bit_pos{opcode_bit_pos} {}
};

#endif
