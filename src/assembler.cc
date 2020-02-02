#include "assembler.hh"


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

bool Token::overlaps(const Token& other)
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

void Assembler::filter_overlapping_tokens(std::vector<Token>& tokens)
{
  auto end = tokens.end() - 1;
  for (auto it = tokens.begin(); it != end; it++)
  {
    const Token& next = *(it+1);

    if (it->overlaps(next))
    {
      if (static_cast<int>(it->type) < static_cast<int>(next.type))
        end = std::remove(it, end, next);
      else
        end = std::remove(it, end, *it);
    }
  }
  tokens.erase(end, tokens.end());
}

std::vector<Token> Assembler::tokenize(void)
{
  std::vector<Token> tokens;
  std::vector<std::string> mnemonics = {
    "mvi", "mov", "lxi", "hlt", "push", "jnz", "cpi"
  };
  std::map<Token::Type, std::string> token_regexes = {
    { Token::Type::DIRECTIVE, R"((\.\w+))" },
    { Token::Type::LABEL, R"((\w+):)" },
    { Token::Type::NUMBER, R"(\b(\d+)\b)" },
    { Token::Type::HEXADECIMAL, R"(0x([a-f\d]+))" },
    { Token::Type::REGISTER, R"(\b([a-ehl]|sp|psw)\b)" },
    { Token::Type::INSTRUCTION, "(" + boost::join(mnemonics, "|") + ")" },
    { Token::Type::SYMBOL, R"(\b([0-9\w]+)\b))" },
  };

  int line_index = 0;
  for (auto line : source)
  {
    std::vector<Token> tmp_tokens;
    for (auto const& [tok_type, tok_rxp] : token_regexes)
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

  filter_overlapping_tokens(tokens);

  for (auto tok : tokens)
    std::cout << tok << std::endl;

  return tokens;
}
