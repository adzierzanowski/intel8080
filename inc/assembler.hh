#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <type_traits>

#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>

#include "cpu.hh"
#include "file_loader.hh"


struct Token
{
  enum class Type : int // precedence, the lower the better
  {
    DIRECTIVE = 0,
    INSTRUCTION = 1,
    HEXADECIMAL = 2,
    BINARY = 3,
    NUMBER = 4,
    REGISTER = 5,
    LABEL = 6,
    SYMBOL = 7
  };

  Type type;
  int line;
  int column;
  std::string value;

  Token(Type type, int line, int column, std::string value);
  bool overlaps(const Token& other) const;

  bool operator !=(const Token& other);
  bool operator ==(const Token& other);
  bool operator <(const Token& other);

  Register get_register() const;
  uint8_t get_uint8() const;
  uint16_t get_uint16() const;
};

std::ostream& operator <<(std::ostream& os, const Token::Type& type_);
std::ostream& operator <<(std::ostream& os, const Token& token);

struct AsmOp
{
  enum class Constraint : uint32_t
  {
    NONE,
    IMM8,
    IMM16,
    BD,
    BDHSP,
    BDHPSW,
    ABCDEHLM,
  };

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

struct Assembler
{
  static const std::map<Token::Type, const std::string> token_regexes;
  static const std::vector<const std::string> mnemonics;
  static std::map<const std::string, AsmOp> instructions;

  static std::vector<Token> filter_overlapping_tokens(std::vector<Token>& tokens);
  static std::vector<Token> convert_numbers(std::vector<Token>& tokens);
  static std::vector<Token> convert_labels(std::vector<Token>& tokens);
  static std::vector<uint8_t> generate_opcodes(std::vector<Token>& tokens);

  static std::vector<Token> tokenize(std::string source);
  static std::vector<Token> tokenize(std::vector<std::string> source_lines);
  static std::vector<uint8_t> assemble(std::string source);
  static std::vector<uint8_t> assemble(std::vector<std::string> source_lines);

};

#endif
