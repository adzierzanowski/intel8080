#include "assembler.hh"

using T = Token::Type;
using C = AsmOp::Constraint;
std::map<const std::string, AsmOp> Assembler::instructions = {
  {"stax", AsmOp(0b00000010, {T::REGISTER}, {C::BD}, {4})},
  {"ldax", AsmOp(0b00001010, {T::REGISTER}, {C::BD}, {4})},
  {"lhld", AsmOp(0b00101010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"shld", AsmOp(0b00100010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"push", AsmOp(0b11000101, {T::REGISTER}, {C::BDHPSW}, {4})},
  {"call", AsmOp(0b11001101, {T::NUMBER}, {C::IMM16}, {-1})},
  {"xthl", AsmOp(0b11100011, {}, {}, {})},
  {"pchl", AsmOp(0b11101001, {}, {}, {})},
  {"xchg", AsmOp(0b11101011, {}, {}, {})},
  {"sphl", AsmOp(0b11111001, {}, {}, {})},

  {"cpi", AsmOp(0b11111110, {T::NUMBER}, {C::IMM8}, {-1})},
  {"nop", AsmOp(0b00000000, {}, {}, {})},
  {"lxi", AsmOp(0b00000001, {T::REGISTER, T::NUMBER}, {C::BDHSP, C::IMM16}, {4, -1})},
  {"inx", AsmOp(0b00000011, {T::REGISTER}, {C::BDHSP}, {4})},
  {"inr", AsmOp(0b00000100, {T::REGISTER}, {C::ABCDEHLM}, {3})},
  {"dcr", AsmOp(0b00000101, {T::REGISTER}, {C::ABCDEHLM}, {3})},
  {"mvi", AsmOp(0b00000110, {T::REGISTER, T::NUMBER}, {C::ABCDEHLM, C::IMM8}, {3, -1})},
  {"rlc", AsmOp(0b00000111, {}, {}, {})},
  {"dad", AsmOp(0b00001001, {T::REGISTER}, {C::BDHSP}, {4})},
  {"dcx", AsmOp(0b00001011, {T::REGISTER}, {C::BDHSP}, {4})},
  {"rrc", AsmOp(0b00001111, {}, {}, {})},
  {"ral", AsmOp(0b00010111, {}, {}, {})},
  {"rar", AsmOp(0b00011111, {}, {}, {})},
  {"daa", AsmOp(0b00100111, {}, {}, {})},
  {"cma", AsmOp(0b00101111, {}, {}, {})},
  {"sta", AsmOp(0b00110010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"stc", AsmOp(0b00110111, {}, {}, {})},
  {"lda", AsmOp(0b00111010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"cmc", AsmOp(0b00111111, {}, {}, {})},
  {"mov", AsmOp(0b01000000, {T::REGISTER, T::REGISTER}, {C::ABCDEHLM, C::ABCDEHLM}, {3, 0})},
  {"hlt", AsmOp(0b01110110, {}, {}, {})},
  {"add", AsmOp(0b10000000, {T::REGISTER}, {C::ABCDEHLM}, {0})},
  {"adc", AsmOp(0b10001000, {T::REGISTER}, {C::ABCDEHLM}, {0})},
  {"sub", AsmOp(0b10010000, {T::REGISTER}, {C::ABCDEHLM}, {0})},
  {"sbb", AsmOp(0b10011000, {T::REGISTER}, {C::ABCDEHLM}, {0})},
  {"ana", AsmOp(0b10100000, {T::REGISTER}, {C::ABCDEHLM}, {0})},
  {"xra", AsmOp(0b10101000, {T::REGISTER}, {C::ABCDEHLM}, {0})},
  {"ora", AsmOp(0b10110000, {T::REGISTER}, {C::ABCDEHLM}, {0})},
  {"cmp", AsmOp(0b10111000, {T::REGISTER}, {C::ABCDEHLM}, {0})},
  {"jmp", AsmOp(0b11000011, {T::NUMBER}, {C::IMM16}, {-1})},
  {"rnz", AsmOp(0b11000000, {}, {}, {})},
  {"pop", AsmOp(0b11000001, {T::REGISTER}, {C::BDHPSW}, {4})},
  {"jnz", AsmOp(0b11000010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"out", AsmOp(0b11010011, {T::NUMBER}, {C::IMM8}, {-1})},
  {"cnc", AsmOp(0b11010100, {T::NUMBER}, {C::IMM16}, {-1})},
  {"sui", AsmOp(0b11010110, {T::NUMBER}, {C::IMM8}, {-1})},
  {"sbi", AsmOp(0b11011110, {T::NUMBER}, {C::IMM8}, {-1})},
  {"jpo", AsmOp(0b11100010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"cpo", AsmOp(0b11100100, {T::NUMBER}, {C::IMM16}, {-1})},
  {"ani", AsmOp(0b11100110, {T::NUMBER}, {C::IMM8}, {-1})},
  {"rpe", AsmOp(0b11101000, {}, {}, {})},
  {"jpe", AsmOp(0b11101010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"cpe", AsmOp(0b11101100, {T::NUMBER}, {C::IMM16}, {-1})},
  {"xri", AsmOp(0b11101110, {T::NUMBER}, {C::IMM8}, {-1})},
  {"ori", AsmOp(0b11110110, {T::NUMBER}, {C::IMM8}, {-1})},
  {"ret", AsmOp(0b11001001, {}, {}, {})},
  {"rnc", AsmOp(0b11010000, {}, {}, {})},
  {"rpo", AsmOp(0b11100000, {}, {}, {})},
  {"jnc", AsmOp(0b11010010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"adi", AsmOp(0b11000110, {T::NUMBER}, {C::IMM8}, {-1})},
  {"aci", AsmOp(0b11001110, {T::NUMBER}, {C::IMM8}, {-1})},
  {"cnz", AsmOp(0b11000100, {T::NUMBER}, {C::IMM16}, {-1})},
  {"rst", AsmOp(0b11000111, {T::NUMBER}, {C::IMM8}, {3})},

  {"rz", AsmOp(0b11001000, {}, {}, {})},
  {"jz", AsmOp(0b11001010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"cz", AsmOp(0b11001100, {T::NUMBER}, {C::IMM16}, {-1})},
  {"rc", AsmOp(0b11011000, {}, {}, {})},
  {"jc", AsmOp(0b11011010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"in", AsmOp(0b11011011, {T::NUMBER}, {C::IMM8}, {-1})},
  {"cc", AsmOp(0b11011100, {T::NUMBER}, {C::IMM16}, {-1})},
  {"rp", AsmOp(0b11110000, {}, {}, {})},
  {"jp", AsmOp(0b11110010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"di", AsmOp(0b11110011, {}, {}, {})},
  {"ei", AsmOp(0b11111011, {}, {}, {})},
  {"cp", AsmOp(0b11110100, {T::NUMBER}, {C::IMM16}, {-1})},
  {"rm", AsmOp(0b11111000, {}, {}, {})},
  {"jm", AsmOp(0b11111010, {T::NUMBER}, {C::IMM16}, {-1})},
  {"cm", AsmOp(0b11111100, {T::NUMBER}, {C::IMM16}, {-1})},
};

const std::vector<const std::string> Assembler::mnemonics = {
  "stax", "ldax", "lhld", "shld", "push", "call", "xthl", "pchl", "xchg", "sphl",
  
  "cpi", "nop", "lxi", "inx", "inr", "dcr", "mvi", "rlc", "dad", "dcx", "rrc",
  "ral", "rar", "daa", "cma", "sta", "stc", "lda", "cmc", "mov", "hlt", "add",
  "adc", "sub", "sbb", "ana", "xra", "ora", "cmp", "rnz", "pop", "jnz", "jmp",
  "cnz", "adi", "rst", "ret", "aci", "rnc", "jnc", "out", "cnc", "sui", "sbi",
  "rpo", "jpo", "cpo", "ani", "rpe", "jpe", "cpe", "xri", "ori",

  "rz", "jz", "cz", "rc", "jc", "in", "cc", "rp", "jp", "di", "cp", "rm", "jm",
  "ei", "cm"
};

const std::map<Token::Type, const std::string> Assembler::token_regexes = {
  { Token::Type::DIRECTIVE, R"((\.\w+))" },
  { Token::Type::LABEL, R"((\w+):)" },
  { Token::Type::NUMBER, R"(\b(\d+)\b)" },
  { Token::Type::HEXADECIMAL, R"(0x([a-f\d]+))" },
  { Token::Type::BINARY, R"(0b([01]+))" },
  { Token::Type::REGISTER, R"(\b([a-ehlm]|sp|psw)\b)" },
  { Token::Type::INSTRUCTION, R"(\b()" + boost::join(mnemonics, "|") + R"()\b)" },
  { Token::Type::SYMBOL, R"(\b([0-9\w]+)\b))" },
};

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

    throw assembler_exception("No register conversion for value '" + value + "'");
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
    case Token::Type::SYMBOL: return "SYMBOL"; break;
    
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

std::vector<Token> Assembler::filter_overlapping_tokens(std::vector<Token>& tokens)
{
  std::vector<Token> filtered;

  for (auto it = tokens.begin(); it != tokens.end(); it++)
  {
    auto tok = *it;

    if (!filtered.empty())
    {
      auto last = filtered.back();

      if (last.overlaps(tok))
      {
        if (tok < last)
        {
          filtered.pop_back();
          filtered.push_back(tok);
        }
      }
      else
      {
        filtered.push_back(tok);
      }
    }
    else
    {
      filtered.push_back(tok);
    }
  }

  return filtered;
}
  
std::vector<Token> Assembler::tokenize(std::string source)
{
  std::vector<std::string> source_lines;
  boost::split(source_lines, source, boost::is_any_of("\n"));
  return tokenize(source_lines);
}

std::vector<Token> Assembler::tokenize(std::vector<std::string> source)
{
  std::vector<Token> tokens;

  int line_index = 0;
  for (auto line : source)
  {
    std::vector<Token> tmp_tokens;
    for (auto const& [tok_type, tok_rxp] : Assembler::token_regexes)
    {
      std::smatch matches;
      std::regex rxp(tok_rxp, std::regex_constants::icase);
      std::regex_token_iterator tokit(line.begin(), line.end(), rxp);
      std::regex_token_iterator<std::string::iterator> end;

      while (tokit != end)
      {
        auto col = std::distance(line.begin(), tokit->first);
        Token tok(tok_type, line_index, col, *tokit);
        tmp_tokens.push_back(tok);
        tokit++;
      }
    }

    // Sort the tokens by position to make filtering overlapping tokens easier.
    std::sort(
      tmp_tokens.begin(),
      tmp_tokens.end(),
      [](const Token& t1, const Token& t2) -> bool
      {
        // Tokens are already parsed line by line so checking just columns
        // works fine.
        return t1.column < t2.column;
      }
    );

    tokens.insert(tokens.end(), tmp_tokens.begin(), tmp_tokens.end());

    line_index++;
  }

  tokens = filter_overlapping_tokens(tokens);

  return tokens;
}

/* Converts numerical tokens into Type::NUMBER token */
std::vector<Token> Assembler::convert_numbers(std::vector<Token>& tokens)
{
  std::vector<Token> converted;
  std::transform(
    tokens.begin(),
    tokens.end(),
    std::back_inserter(converted),
    [](const Token& tok) {
      if (tok.type == Token::Type::HEXADECIMAL)
      {
        unsigned int tokval = std::stoul(
          tok.value.substr(2, tok.value.length()-2),
          nullptr,
          16
        );
        return Token(Token::Type::NUMBER, tok.line, tok.column, std::to_string(tokval));
      }
      else if (tok.type == Token::Type::BINARY)
      {
        unsigned int tokval = std::stoul(
          tok.value.substr(2, tok.value.length()-2),
          nullptr,
          2
        );
        return Token(Token::Type::NUMBER, tok.line, tok.column, std::to_string(tokval));
      }

      return tok;
    }
  );

  return converted;
}

std::vector<uint8_t> Assembler::generate_opcodes(std::vector<Token>& tokens)
{
  std::vector<uint8_t> binary;

  for (auto it = tokens.begin(); it != tokens.end(); it++)
  {
    const Token& tok = *it;
    std::cout << "ASSEMBLING " << tok << std::endl;

    bool last_token = (it == (tokens.end() - 1));

    switch (tok.type)
    {
      case Token::Type::INSTRUCTION:
      {
        auto op = instructions[tok.value];
        int argc = op.operands.size();

        if (argc > 0 && last_token)
        {
          throw assembler_exception(boost::str(
              boost::format("%d:%d Instruction %s expects %d argument(s)")
              % tok.line
              % tok.column
              % tok.value
              % argc
          ));
        }

        std::vector<uint8_t> result = {op.opcode_template};

        // Iterate over expected operands
        for (int i = 0; i < argc; i++)
        {
          auto operand_type = op.operands[i];
          auto constraint = op.constraints[i];
          auto bit_shift = op.opcode_bit_pos[i];
          const Token& optok = *(it+i+1); // Fetch operand token

          std::cout << "OPERAND " << i << " " << optok << std::endl;

          if (operand_type != optok.type)
          {
            throw assembler_exception(boost::str(
              boost::format(
                "%d:%d Instruction %s expects operand #%d to be of type %s, not %s"
              ) % tok.line % tok.column % tok.value % (i+1) % operand_type % optok.type
            ));
          }

          switch (operand_type)
          {
            case Token::Type::REGISTER:
            {
              // If the operand alters the instruction opcode
              if (bit_shift > -1)
              {
                uint8_t register_value;
                if (constraint == AsmOp::Constraint::BD
                  || constraint == AsmOp::Constraint::BDHSP
                  || constraint == AsmOp::Constraint::BDHPSW)
                {
                  register_value = static_cast<uint8_t>(
                    to_regpair(optok.get_register())
                  );
                }
                else if (constraint == AsmOp::Constraint::ABCDEHLM)
                {
                  register_value = static_cast<uint8_t>(optok.get_register());
                }
                else
                {
                  throw assembler_exception(boost::str(
                    boost::format(
                      "%d:%d Wrong constraint for instruction '%s'"
                    ) % tok.line % tok.column % tok.value
                  ));
                }

                std::cout << "ALTER REGISTER_VALUE=";
                std::cout << +register_value;
                std::cout << " TEMPLATE=" << +op.opcode_template;
                std::cout << " BIT_SHIFT=" << +bit_shift << std::endl;

                // This is an ugly hack:
                // In order to prevent duplicates in the Register enum,
                // SP is defined to 0b1011 although it really should be 0b11
                // so we have to & it with 0b111 mask to get the real thing.
                result[0] |= (register_value & 0b111) << bit_shift;
              }

              break;
            }
            case Token::Type::NUMBER:
            {

              if (constraint == AsmOp::Constraint::IMM8)
              {
                if (bit_shift > -1)
                {
                  result[0] |= (optok.get_uint8() << bit_shift);
                }
                else
                {
                  result.push_back(optok.get_uint8());
                }
              }
              else if (constraint == AsmOp::Constraint::IMM16)
              {
                uint16_t val = optok.get_uint16();
                // Little-endian
                result.push_back(val & 0x00ff);
                result.push_back((val & 0xff00) >> 8);
              }
              else
              {
                throw assembler_exception("Wrong constraint for NUMBER");
              }
              break;
            }

            default:
              throw assembler_exception(
                "Wrong type for instruction operand: '"
                + to_string(operand_type) + "'"
              );
              break;
          }
        }

        binary.insert(binary.end(), result.begin(), result.end());

        std::cout << std::endl;

        // Next token after the instruction AND its operands
        it += argc;

        break;
      }

      default:
        break;
    }
  }

  return binary;
}

std::vector<Token> Assembler::convert_labels(std::vector<Token>& tokens)
{
  std::vector<Token> converted;
  std::map<std::string, uint16_t> labels;

  unsigned int origin = 0;
  uint16_t token_position = 0;

  for (auto it = tokens.begin(); it != tokens.end(); it++)
  {
    const Token& tok = *it;
    bool last_token = (it == (tokens.end() - 1));

    if (tok.type == Token::Type::DIRECTIVE && tok.value == ".org")
    {
      if (last_token)
      {
        throw assembler_exception("Directive .org expects an argument");
      }

      const Token& next = *(it + 1);
      origin = std::stoul(next.value);
      if (origin > 0xffff)
      {
        throw assembler_exception("Directive .org expects a 16-bit value");
      }

      converted.push_back(tok);

      // Move past the directive argument to not increase token_position
      it++;
      continue;
    }
    else if (tok.type == Token::Type::LABEL)
    {
      auto label = labels.find(tok.value);

      if (label != labels.end())
      {
        throw assembler_exception(
          "Found two labels with the same name: '" + tok.value + "'"
        );
      }

      labels.insert({tok.value, origin + token_position});
      converted.push_back(tok);
    }
    else if (tok.type == Token::Type::SYMBOL)
    {
      auto label = labels.find(tok.value + ":");

      if (label == labels.end())
      {
        throw assembler_exception("Undefined symbol: '" + tok.value + "'");
      }

      Token new_token = tok; 
      new_token.type = Token::Type::NUMBER;
      new_token.value = std::to_string(labels[tok.value + ":"]);
      converted.push_back(new_token);
    }
    else
    {
      converted.push_back(tok);
    }

    if (tok.type == Token::Type::NUMBER)
    {
      unsigned int val = std::stoul(tok.value);
      if (val > 0xffff)
      {
        throw assembler_exception("Address exceeds 16 bits");
      }
      token_position += val > 0xff ? 2 : 1;
    }
    else if (tok.type == Token::Type::INSTRUCTION)
    {
      token_position++;
    }
  }

  return converted;
}

std::vector<uint8_t> Assembler::assemble(std::string source)
{
  auto tokens = tokenize(source);
  tokens = convert_numbers(tokens);
  tokens = convert_labels(tokens);
  return generate_opcodes(tokens);
}

std::vector<uint8_t> Assembler::assemble(std::vector<std::string> source_lines)
{
  auto tokens = tokenize(source_lines);
  tokens = convert_numbers(tokens);
  tokens = convert_labels(tokens);
  return generate_opcodes(tokens);
}
