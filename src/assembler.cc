#include "assembler.hh"


using T = Token::Type;
using C = Constraint;

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
  {"lxi", AsmOp(
    0b00000001, {T::REGISTER, T::NUMBER}, {C::BDHSP, C::IMM16}, {4, -1}
  )},
  {"inx", AsmOp(0b00000011, {T::REGISTER}, {C::BDHSP}, {4})},
  {"inr", AsmOp(0b00000100, {T::REGISTER}, {C::ABCDEHLM}, {3})},
  {"dcr", AsmOp(0b00000101, {T::REGISTER}, {C::ABCDEHLM}, {3})},
  {"mvi", AsmOp(
    0b00000110, {T::REGISTER, T::NUMBER}, {C::ABCDEHLM, C::IMM8}, {3, -1}
  )},
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
  {"mov", AsmOp(
    0b01000000, {T::REGISTER, T::REGISTER}, {C::ABCDEHLM, C::ABCDEHLM}, {3, 0}
  )},
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
  { Token::Type::DIRECTIVE, R"(\.\w+)" },
  { Token::Type::LABEL, R"(\w+:)" },
  { Token::Type::NUMBER, R"(\b\d+\b)" },
  { Token::Type::HEXADECIMAL, R"((0x[a-f\d]+|[a-f\d]+h))" },
  { Token::Type::BINARY, R"(0b[01]+)" },
  { Token::Type::REGISTER, R"(\b([a-ehlm]|sp|psw)\b)" },
  { Token::Type::INSTRUCTION, R"(\b()" + boost::join(mnemonics, "|") + R"()\b)" },
  { Token::Type::IDENTIFIER, R"(\b[0-9\w]+\b)" },
  { Token::Type::COMMENT, R"(;.+)" },
  { Token::Type::STRING, R"(("|').*?("|'))" },
  { Token::Type::DATA, R"(\b(db|dw)\b)" },
};



std::vector<Token> Assembler::filter_overlapping_tokens(
  std::vector<Token>& tokens
) {
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
        unsigned int tokval;

        if (tok.value.back() == 'h')
        {
          tokval = std::stoul(
            tok.value.substr(0, tok.value.length()-1),
            nullptr,
            16
          );
        }
        else
        {
          tokval = std::stoul(
            tok.value.substr(2, tok.value.length()-2),
            nullptr,
            16
          );
        }

        return Token(
          Token::Type::NUMBER, tok.line, tok.column, std::to_string(tokval));
      }
      else if (tok.type == Token::Type::BINARY)
      {
        unsigned int tokval = std::stoul(
          tok.value.substr(2, tok.value.length()-2),
          nullptr,
          2
        );

        return Token(
          Token::Type::NUMBER, tok.line, tok.column, std::to_string(tokval));
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
              boost::format(
                "%d:%d Instruction %s expects %d argument(s)"
              ) % tok.line
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

          if (!optok.constraint_fulfilled(constraint))
          {
            throw assembler_exception(boost::str(
              boost::format(
                "%d:%d Constraint unfulfilled: operand #%d expects %s: %s"
              ) % tok.line
                % tok.column
                % i
                % to_string(operand_type)
                % to_string(constraint)
            ));
          }

          if (operand_type != optok.type)
          {
            throw assembler_exception(boost::str(
              boost::format(
                "%d:%d Instruction %s expects operand #%d "
                "to be of type %s, not %s"
              ) % tok.line
                % tok.column
                % tok.value
                % (i+1)
                % operand_type
                % optok.type
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
                if (constraint == Constraint::BD
                  || constraint == Constraint::BDHSP
                  || constraint == Constraint::BDHPSW)
                {
                  register_value = static_cast<uint8_t>(
                    to_regpair(optok.get_register())
                  );
                }
                else if (constraint == Constraint::ABCDEHLM)
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

              if (constraint == Constraint::IMM8)
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
              else if (constraint == Constraint::IMM16)
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

        // Next token after the instruction AND its operands
        it += argc;

        break;
      }

      case Token::Type::DATA:
      {
        auto data_it = it;

        while (true)
        {
          const Token& data_tok = *(++data_it);

          switch (data_tok.type)
          {
            case Token::Type::STRING:
            {
              if (tok.value == "db")
              {
                std::string payload = data_tok.value.substr(
                  1, data_tok.value.length() - 2
                );

                for (auto c : payload)
                {
                  binary.push_back(static_cast<uint8_t>(c));
                }
              }
              else
              {
                throw assembler_exception(boost::str(boost::format(
                    "%d:%d String data must be of byte width"
                  ) % data_tok.line
                    % data_tok.column
                ));
              }

              break;
            }

            case Token::Type::NUMBER:
            {
              if (tok.value == "db")
              {
                binary.push_back(data_tok.get_uint8());
              }
              else if (tok.value == "dw")
              {
                auto val = data_tok.get_uint16();
                binary.push_back(val & 0x00ff);
                binary.push_back((val & 0xff00) >> 8);
              }

              break;
            }

            default:
            {
              throw assembler_exception(boost::str(boost::format(
                  "%d:%d Data definition expects number or string literals "
                  "not %s"
                ) % data_tok.line
                  % data_tok.column
                  % to_string(data_tok.type)
              ));

              break;
            }
          }

          if (data_it >= tokens.end() - 1 || tok.line != data_tok.line)
            break;
        }

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
  uint16_t label_position = 0; // label address in code

  // Fill the label map
  for (auto it = tokens.begin(); it != tokens.end(); it++)
  {
    const Token& tok = *it;
    bool last_token = (it == (tokens.end() - 1));

    switch (tok.type)
    {
      case Token::Type::LABEL:
      {
        if (labels.find(tok.value) != labels.end())
        {
          throw assembler_exception(boost::str(boost::format(
            "%d:%d Duplicate label: '%s'."
            ) % tok.line % tok.column % tok.value
          ));
        }

        labels.insert({tok.value, origin + label_position});
        break;
      }

      case Token::Type::DIRECTIVE:
      {
        if (tok.value == ".org")
        {
          if (last_token)
          {
            throw assembler_exception(boost::str(boost::format(
              "%d:%d .org directive expects an argument."
              ) % tok.line % tok.column
            ));
          }

          const Token& orgval = *(it+1);
          if (orgval.type != Token::Type::NUMBER)
          {
            throw assembler_exception(boost::str(boost::format(
              "%d:%d .org directive expects a number as an argument, not %s."
              ) % tok.line % tok.column % to_string(tok.type)
            ));
          }

          origin = std::stoul(orgval.value);
          it++;
        }
        break;
      }

      case Token::Type::INSTRUCTION:
      {
        auto ins = instructions[tok.value];
        //it += ins.operands.size() - 1;
        label_position++;
        break;
      }

      case Token::Type::NUMBER:
      {
        label_position += std::stoul(tok.value) > 0xff ? 2 : 1;
        break;
      }

      case Token::Type::IDENTIFIER:
      {
        label_position += 2;
        break;
      }

      default:
        break;
    }
  }

  for (auto tok : tokens)
  {
    if (tok.type == Token::Type::IDENTIFIER)
    {
      auto label = labels.find(tok.value + ":");

      if (label == labels.end())
      {
        throw assembler_exception(boost::str(boost::format(
            "%d:%d Undefined identifier: %s"
          ) % tok.line % tok.column % tok.value
        ));
      }

      Token convtok(tok);
      convtok.type = Token::Type::NUMBER;
      convtok.value = std::to_string(label->second);
      converted.push_back(convtok);
    }
    else
    {
      converted.push_back(tok);
    }
  }

  return converted;
}

std::vector<Token> Assembler::convert_case(std::vector<Token>& tokens)
{
  std::vector<Token> converted;

  for (auto tok : tokens)
  {
    switch (tok.type)
    {
      case Token::Type::COMMENT:
      case Token::Type::IDENTIFIER:
      case Token::Type::LABEL:
      case Token::Type::STRING:
      {
        converted.push_back(tok);
        break;
      }

      default:
      {
        Token t(tok);
        std::transform(
          t.value.begin(),
          t.value.end(),
          t.value.begin(),
          [] (auto c) {
            return tolower(c);
        });
        converted.push_back(t);
        break;
      }
    }
  }

  return converted;
}

std::vector<uint8_t> Assembler::assemble(std::string source)
{
  auto tokens = tokenize(source);
  tokens = convert_numbers(tokens);
  tokens = convert_labels(tokens);
  tokens = convert_case(tokens);
  return generate_opcodes(tokens);
}

std::vector<uint8_t> Assembler::assemble(std::vector<std::string> source_lines)
{
  auto tokens = tokenize(source_lines);
  tokens = convert_numbers(tokens);
  tokens = convert_labels(tokens);
  tokens = convert_case(tokens);
  return generate_opcodes(tokens);
}
