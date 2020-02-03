#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <regex>
#include <map>

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

  bool operator!=(const Token& other);
  bool operator==(const Token& other);
  bool operator<(const Token& other);
};

std::ostream& operator<<(std::ostream& os, const Token::Type& type_);
std::ostream& operator<<(std::ostream& os, const Token& token);

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

  const std::string mnemonic;
  const std::vector<Token::Type> operands;
  const std::vector<Constraint> constraints;

  AsmOp() {}
  AsmOp(
    std::initializer_list<Token::Type> operands,
    std::initializer_list<Constraint> constraints
  )
    : operands{operands}, constraints{constraints} {}
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
