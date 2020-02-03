#include "assembler.hh"


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
  { Token::Type::REGISTER, R"(\b([a-ehlm]|sp|psw)\b)" },
  { Token::Type::INSTRUCTION, "(" + boost::join(mnemonics, "|") + ")" },
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

std::ostream& operator<<(std::ostream &os, const Token::Type& type_)
{
  switch (type_)
  {
    case Token::Type::INSTRUCTION: os << "INSTRUCTION"; break;
    case Token::Type::DIRECTIVE: os << "DIRECTIVE"; break;
    case Token::Type::NUMBER: os << "NUMBER"; break;
    case Token::Type::HEXADECIMAL: os << "HEXADECIMAL"; break;
    case Token::Type::LABEL: os << "LABEL"; break;
    case Token::Type::REGISTER: os << "REGISTER"; break;
    case Token::Type::SYMBOL: os << "SYMBOL"; break;
    
    default:
      os << "<UNKNOWN>";
      break;
  }

  return os;
}

std::ostream& operator<<(std::ostream &os, const Token& token)
{
  os << boost::format("Token(%s, \"%s\") @ pos(%d,%d)")
    % token.type % token.value % token.line % token.column;
  return os;
}

Assembler::Assembler() {}

void Assembler::load_lines(std::vector<std::string> lines)
{
  source = lines;
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
  

std::vector<Token> Assembler::tokenize(void)
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

    std::sort(
      tmp_tokens.begin(),
      tmp_tokens.end(),
      [](const Token& t1, const Token& t2) -> bool
      {
        return t1.column < t2.column;
      }
    );

    tokens.insert(tokens.end(), tmp_tokens.begin(), tmp_tokens.end());

    line_index++;
  }

  tokens = filter_overlapping_tokens(tokens);

  return tokens;
}

std::vector<uint8_t> Assembler::assemble(std::vector<Token>& tokens)
{
  std::vector<uint8_t> binary = {1,2,3};

  for (auto it = tokens.begin(); it != tokens.end(); it++)
  {
    const Token &tok = *it;
    bool last_token = it == tokens.end() - 1;
    switch (tok.type)
    {
      case Token::Type::INSTRUCTION:
        if (tok.value == "mvi")
        {
          if (last_token)
          {
            std::cerr << tok.line << ":" << tok.column << " ";
            std::cerr << "Instruction mvi expects two operands." << std::endl;
          }

          const Token& dst = *(it+1);
          const Token& imm8 = *(it+2);

          if (dst.type != Token::Type::REGISTER)
          {
            std::cerr << tok.line << ":" << tok.column << " ";
            std::cerr << "Instruction mvi expects REGISTER to be the first operand." << std::endl;
          }

          if (imm8.type != Token::Type::NUMBER)
          {
            std::cerr << tok.line << ":" << tok.column << " ";
            std::cerr << "Instruction mvi expects NUMBER to be the second operand." << std::endl;
          }

          int imm8_val = std::stoul(imm8.value);

          if (imm8_val > 0xff)
          {
            std::cerr << tok.line << ":" << tok.column << " ";
            std::cerr << "Instruction mvi expects second operand to be 8-bit immediate." << std::endl;
          }
        }

        break;
    }
  }

  return binary;
}
