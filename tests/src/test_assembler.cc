#include "test_assembler.hh"

void test_assembler_init(void)
{
  assembler = std::make_unique<Assembler>();
}

void test_assembler_fini(void)
{
  assembler.reset();
}

Test(assembler, test_token_eq, .init=test_assembler_init, .fini=test_assembler_fini)
{
  Token t1(Token::Type::INSTRUCTION, 3, 4, "foo");
  Token t2(Token::Type::INSTRUCTION, 3, 4, "foo");
  Token t3(Token::Type::INSTRUCTION, 8, 4, "foo");

  cr_assert_eq(t1, t2);
  cr_assert_neq(t1, t3);
}

Test(assembler, test_tokenize, .init=test_assembler_init, .fini=test_assembler_fini)
{
  std::vector<std::string> src;
  src.push_back("\t.org 0x100 .foo .bar 2");
  src.push_back("mvi a,     55");
  src.push_back("    ");
  src.push_back("mov a, c");
  src.push_back("mov b,c");
  src.push_back("hlt lxi sp, 0x44ae");
  src.push_back("    ");
  src.push_back("label:");
  src.push_back("push psw");
  src.push_back("cpi 8");
  src.push_back("jnz label");

  std::vector<Token> expected_tokens = {
    Token(Token::Type::DIRECTIVE, 0, 1, ".org"),
    Token(Token::Type::HEXADECIMAL, 0, 6, "0x100"),
    Token(Token::Type::DIRECTIVE, 0, 12, ".foo"),
    Token(Token::Type::DIRECTIVE, 0, 17, ".bar"),
    Token(Token::Type::NUMBER, 0, 22, "2"),

    Token(Token::Type::INSTRUCTION, 1, 0, "mvi"),
    Token(Token::Type::REGISTER, 1, 4, "a"),
    Token(Token::Type::NUMBER, 1, 11, "55"),

    Token(Token::Type::INSTRUCTION, 3, 0, "mov"),
    Token(Token::Type::REGISTER, 3, 4, "a"),
    Token(Token::Type::REGISTER, 3, 7, "c"),

    Token(Token::Type::INSTRUCTION, 4, 0, "mov"),
    Token(Token::Type::REGISTER, 4, 4, "b"),
    Token(Token::Type::REGISTER, 4, 6, "c"),

    Token(Token::Type::INSTRUCTION, 5, 0, "hlt"),
    Token(Token::Type::INSTRUCTION, 5, 4, "lxi"),
    Token(Token::Type::REGISTER, 5, 8, "sp"),
    Token(Token::Type::HEXADECIMAL, 5, 12, "0x44ae"),

    Token(Token::Type::LABEL, 7, 0, "label:"),

    Token(Token::Type::INSTRUCTION, 8, 0, "push"),
    Token(Token::Type::REGISTER, 8, 5, "psw"),

    Token(Token::Type::INSTRUCTION, 9, 0, "cpi"),
    Token(Token::Type::NUMBER, 9, 4, "8"),

    Token(Token::Type::INSTRUCTION, 10, 0, "jnz"),
    Token(Token::Type::SYMBOL, 10, 4, "label"),
  };


  assembler->load_lines(src);

  auto tokens = assembler->tokenize();

  cr_assert_eq(tokens.size(), expected_tokens.size());
  for (int i = 0; i < tokens.size(); i++)
  {
    cr_assert_eq(tokens[i], expected_tokens[i]);
  }
}
