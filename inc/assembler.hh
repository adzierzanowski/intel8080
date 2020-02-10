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

#include "asmconstraint.hh"
#include "asmexception.hh"
#include "asmoperand.hh"
#include "cpu.hh"
#include "file_loader.hh"
#include "token.hh"


struct Assembler
{
  static const std::map<Token::Type, const std::string> token_regexes;
  static const std::vector<const std::string> mnemonics;
  static std::map<const std::string, AsmOp> instructions;

  static std::vector<Token> filter_overlapping_tokens(std::vector<Token>& tokens);
  static std::vector<Token> convert_numbers(std::vector<Token>& tokens);
  static std::vector<Token> convert_labels(std::vector<Token>& tokens);
  static std::vector<Token> convert_case(std::vector<Token>& tokens);
  static std::vector<uint8_t> generate_opcodes(std::vector<Token>& tokens);

  static std::vector<Token> tokenize(std::string source);
  static std::vector<Token> tokenize(std::vector<std::string> source_lines);
  static std::vector<uint8_t> assemble(std::string source);
  static std::vector<uint8_t> assemble(std::vector<std::string> source_lines);
};

#endif
