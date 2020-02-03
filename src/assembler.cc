#include "assembler.hh"

using T = Token::Type;
using C = AsmOp::Constraint;
std::map<const std::string, AsmOp> Assembler::instructions = {
  {"stax", AsmOp({T::REGISTER}, {C::BD})},
  {"ldax", AsmOp({T::REGISTER}, {C::BD})},
  {"lhld", AsmOp({T::NUMBER}, {C::IMM16})},
  {"shld", AsmOp({T::NUMBER}, {C::IMM16})},
  {"push", AsmOp({T::REGISTER}, {C::BDHPSW})},
  {"call", AsmOp({T::NUMBER}, {C::IMM16})},
  {"xthl", AsmOp({}, {})},
  {"pchl", AsmOp({}, {})},
  {"xchg", AsmOp({}, {})},
  {"sphl", AsmOp({}, {})},

  {"cpi", AsmOp({T::NUMBER}, {C::IMM8})},
  {"nop", AsmOp({}, {})},
  {"lxi", AsmOp({T::NUMBER}, {C::IMM16})},
  {"inx", AsmOp({T::REGISTER}, {C::BDHSP})},
  {"inr", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"dcr", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"mvi", AsmOp({T::REGISTER, T::NUMBER}, {C::ABCDEHLM, C::IMM8})},
  {"rlc", AsmOp({}, {})},
  {"dad", AsmOp({T::REGISTER}, {C::BDHSP})},
  {"dcx", AsmOp({T::REGISTER}, {C::BDHSP})},
  {"rrc", AsmOp({}, {})},
  {"ral", AsmOp({}, {})},
  {"rar", AsmOp({}, {})},
  {"daa", AsmOp({}, {})},
  {"daa", AsmOp({}, {})},
  {"cma", AsmOp({}, {})},
  {"sta", AsmOp({T::NUMBER}, {C::IMM16})},
  {"stc", AsmOp({}, {})},
  {"lda", AsmOp({T::NUMBER}, {C::IMM16})},
  {"cmc", AsmOp({}, {})},
  {"mov", AsmOp({T::REGISTER, T::REGISTER}, {C::ABCDEHLM, C::ABCDEHLM})},
  {"hlt", AsmOp({}, {})},
  {"add", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"adc", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"sub", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"sbb", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"ana", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"xra", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"ora", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"cmp", AsmOp({T::REGISTER}, {C::ABCDEHLM})},
  {"rnz", AsmOp({}, {})},
  {"pop", AsmOp({T::REGISTER}, {C::BDHPSW})},
  {"jnz", AsmOp({T::NUMBER}, {C::IMM16})},
  {"out", AsmOp({T::NUMBER}, {C::IMM8})},
  {"cnc", AsmOp({T::NUMBER}, {C::IMM16})},
  {"sui", AsmOp({T::NUMBER}, {C::IMM8})},
  {"sbi", AsmOp({T::NUMBER}, {C::IMM8})},
  {"rpo", AsmOp({}, {})},
  {"jpo", AsmOp({T::NUMBER}, {C::IMM16})},
  {"cpo", AsmOp({T::NUMBER}, {C::IMM16})},
  {"ani", AsmOp({T::NUMBER}, {C::IMM8})},
  {"rpe", AsmOp({}, {})},
  {"jpe", AsmOp({T::NUMBER}, {C::IMM16})},
  {"cpe", AsmOp({T::NUMBER}, {C::IMM16})},
  {"xri", AsmOp({T::NUMBER}, {C::IMM8})},
  {"ori", AsmOp({T::NUMBER}, {C::IMM8})},

  {"rz", AsmOp({}, {})},
  {"jz", AsmOp({T::NUMBER}, {C::IMM16})},
  {"cz", AsmOp({T::NUMBER}, {C::IMM16})},
  {"rc", AsmOp({}, {})},
  {"jc", AsmOp({T::NUMBER}, {C::IMM16})},
  {"in", AsmOp({T::NUMBER}, {C::IMM8})},
  {"cc", AsmOp({T::NUMBER}, {C::IMM16})},
  {"rp", AsmOp({}, {})},
  {"jp", AsmOp({T::NUMBER}, {C::IMM16})},
  {"di", AsmOp({}, {})},
  {"ei", AsmOp({}, {})},
  {"cp", AsmOp({T::NUMBER}, {C::IMM16})},
  {"rm", AsmOp({}, {})},
  {"jm", AsmOp({T::NUMBER}, {C::IMM16})},
  {"cm", AsmOp({T::NUMBER}, {C::IMM16})},
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

std::ostream& operator<<(std::ostream &os, const Token::Type& type_)
{
  switch (type_)
  {
    case Token::Type::INSTRUCTION: os << "INSTRUCTION"; break;
    case Token::Type::DIRECTIVE: os << "DIRECTIVE"; break;
    case Token::Type::NUMBER: os << "NUMBER"; break;
    case Token::Type::HEXADECIMAL: os << "HEXADECIMAL"; break;
    case Token::Type::BINARY: os << "BINARY"; break;
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
          std::cerr << tok.line << ":" << tok.column << " ";
          std::cerr << "Instruction '" << tok.value << "' ";
          std::cerr << "expects " << argc << " argument(s).\n";
          return binary;
        }

        for (int i = 0; i < argc; i++)
        {
          const Token& optok = *(it+i+1);
          std::cout << "OPERAND " << i << " " << optok << std::endl;

          if (op.operands[i] != optok.type)
          {
            std::cerr << tok.line << ":" << tok.column << " ";
            std::cerr << "Instruction '" << tok.value << "' ";
            std::cerr << "expects operand #" << (i+1) << " to be of type ";
            std::cerr << op.operands[i] << ", not " << optok.type << ".\n";

            return binary;
          }
        }
        std::cout << std::endl;

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
        std::cerr << "Directive .org expects an argument.\n";
        return converted;
      }

      const Token& next = *(it + 1);
      origin = std::stoul(next.value);
      if (origin > 0xffff)
      {
        std::cerr << "Directive .org expects a 16-bit value." << std::endl;
        return converted;
      }

      converted.push_back(tok);
    }
    else if (tok.type == Token::Type::LABEL)
    {
      auto label = labels.find(tok.value);

      if (label != labels.end())
      {
        std::cerr << "Found two labels with the same name: '" << tok.value << "'.";
        return converted;
      }

      labels.insert({tok.value, origin + token_position});
      converted.push_back(tok);
    }
    else if (tok.type == Token::Type::SYMBOL)
    {
      auto label = labels.find(tok.value + ":");

      if (label == labels.end())
      {
        std::cerr << "No such label: '" << tok.value << "'.\n";
        return converted;
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

    token_position++;
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
