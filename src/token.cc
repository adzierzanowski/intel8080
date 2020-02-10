#include "token.hh"


Token::Token(Type type, int line, int column, std::string value)
    : type{type}, line{line}, column{column}, value{value} {}

bool Token::operator==(const Token& other)
{
  return type == other.type
    && line == other.line
    && column == other.column
    && value == other.value;
}

bool Token::operator!=(const Token& other)
{
  return ! (*this == other);
}

bool Token::operator<(const Token& other)
{
  return static_cast<int>(type) < static_cast<int>(other.type);
}

bool Token::overlaps(const Token& other) const
{
  int start1 = column;
  int end1 = column + value.length();
  int start2 = other.column;
  int end2 = other.column + other.value.length();

  return (line == other.line)
    && ((start2 <= start1 && end2 >= start1)
      || (start2 >= start1 && start2 <= end1));
}

Register Token::get_register() const
{
  if (type == Token::Type::REGISTER)
  {
    if (value == "a") return Register::A;
    if (value == "b") return Register::B;
    if (value == "c") return Register::C;
    if (value == "d") return Register::D;
    if (value == "e") return Register::E;
    if (value == "h") return Register::H;
    if (value == "l") return Register::L;
    if (value == "m") return Register::M;
    if (value == "sp") return Register::SP;

    // In this case, the opcode alteration is the same as with SP
    if (value == "psw") return Register::SP;

    throw assembler_exception(
      "No register conversion for value '" + value + "'"
    );
  }
  else
  {
    throw assembler_exception("get_register expects REGISTER type");
  }
}

uint8_t Token::get_uint8() const
{
  // TODO: check the boundary
  return std::stoul(value);
}

uint16_t Token::get_uint16() const
{
  // TODO: check the boundary
  return std::stoul(value);
}

bool Token::constraint_fulfilled(Constraint constraint) const
{
  switch (constraint)
  {
    case Constraint::ABCDEHLM:
    {
      if (type != Token::Type::REGISTER)
        return false;

      std::vector<std::string> valid_values = {
        "a", "b", "c", "d", "e", "h", "l", "m"
      };
      if (std::find(
        valid_values.begin(), valid_values.end(), value) != valid_values.end())
      {
        return true;
      }
      return false;

      break;
    }
    case Constraint::BD:
    {
      if (type != Token::Type::REGISTER)
        return false;

      std::vector<std::string> valid_values = {"b", "d"};
      if (std::find(
        valid_values.begin(), valid_values.end(), value) != valid_values.end())
      {
        return true;
      }
      return false;

      break;
    };
    case Constraint::BDHPSW:
    {
      if (type != Token::Type::REGISTER)
        return false;

      std::vector<std::string> valid_values = {"b", "d", "h", "psw"};
      if (std::find(
        valid_values.begin(), valid_values.end(), value) != valid_values.end())
      {
        return true;
      }
      return false;

      break;
    }
    case Constraint::BDHSP:
    {
      if (type != Token::Type::REGISTER)
        return false;

      std::vector<std::string> valid_values = {"b", "d", "h", "sp"};
      if (std::find(
        valid_values.begin(), valid_values.end(), value) != valid_values.end())
      {
        return true;
      }
      return false;

      break;
    }
    case Constraint::IMM8:
    {
      if (type != Token::Type::NUMBER)
      {
        return false;
      };

      unsigned int intval = std::stoul(value);
      if (intval > 0xff)
      {
        return false;
      }
      return true;
    }
    case Constraint::IMM16:
    {
      if (type != Token::Type::NUMBER)
      {
        return false;
      };

      unsigned int intval = std::stoul(value);
      if (intval > 0xffff)
      {
        return false;
      }
      return true;
    };
    default:
    {
      return true;
      break;
    }
  }
}

std::string to_string(const Token::Type& type_)
{
  switch (type_)
  {
    case Token::Type::INSTRUCTION: return "INSTRUCTION"; break;
    case Token::Type::DIRECTIVE: return "DIRECTIVE"; break;
    case Token::Type::NUMBER: return "NUMBER"; break;
    case Token::Type::HEXADECIMAL: return "HEXADECIMAL"; break;
    case Token::Type::BINARY: return "BINARY"; break;
    case Token::Type::LABEL: return "LABEL"; break;
    case Token::Type::REGISTER: return "REGISTER"; break;
    case Token::Type::IDENTIFIER: return "IDENTIFIER"; break;
    case Token::Type::COMMENT: return "COMMENT"; break;
    case Token::Type::STRING: return "STRING"; break;
    case Token::Type::DATA: return "DATA"; break;
    
    default:
      return "<UNKNOWN>";
      break;
  }
}

std::ostream& operator<<(std::ostream &os, const Token::Type& type_)
{
  os << to_string(type_);
  return os;
}

std::ostream& operator<<(std::ostream &os, const Token& token)
{
  os << boost::format("Token(%s, \"%s\") @ pos(%d,%d)")
    % token.type % token.value % token.line % token.column;
  return os;
}
