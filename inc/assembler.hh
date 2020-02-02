#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <regex>
#include <map>

#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

#include "cpu.hh"
#include "file_loader.hh"


struct Token
{
  enum class Type : int // precedence, the lower the better
  {
    DIRECTIVE = 0,
    INSTRUCTION = 1,
    HEXADECIMAL = 2,
    NUMBER = 3,
    REGISTER = 4,
    LABEL = 5,
    SYMBOL = 6
  };

  Type type;
  int line;
  int column;
  std::string value;

  Token(Type type, int line, int column, std::string value);
  bool overlaps(const Token& other);

  bool operator!=(const Token& other);
  bool operator==(const Token& other);
};

std::ostream& operator<<(std::ostream& os, const Token::Type& type_);
std::ostream& operator<<(std::ostream& os, const Token& token);

class Assembler
{
  std::vector<std::string> source;
  void filter_overlapping_tokens(std::vector<Token>& tokens);

  public:
    Assembler();
    std::vector<Token> tokenize(void);
    std::vector<uint8_t> assemble(void);
    void load_text(std::string src);
    void load_file(std::string filename);
    void load_lines(std::vector<std::string> lines);
};

#endif
