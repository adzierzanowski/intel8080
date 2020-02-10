#ifndef TOKEN_H
#define TOKEN_H

#include <boost/format.hpp>

#include "asmconstraint.hh"
#include "asmexception.hh"


struct Token
{
  enum class Type : int // position = precedence, the lower the better
  {
    COMMENT,
    DIRECTIVE,
    INSTRUCTION,
    STRING,
    HEXADECIMAL,
    BINARY,
    NUMBER,
    REGISTER,
    DATA,
    LABEL,
    IDENTIFIER,
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

  bool constraint_fulfilled(Constraint constraint) const;
};

std::string to_string(const Token::Type& type_);
std::ostream& operator <<(std::ostream& os, const Token::Type& type_);
std::ostream& operator <<(std::ostream& os, const Token& token);

#endif
