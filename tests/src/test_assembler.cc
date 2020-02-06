#include "test_assembler.hh"


void test_assembler_init(void)
{
}

void test_assembler_fini(void)
{
}

Test(assembler, test_token_eq, .init=test_assembler_init, .fini=test_assembler_fini)
{
  Token t1(Token::Type::INSTRUCTION, 3, 4, "foo");
  Token t2(Token::Type::INSTRUCTION, 3, 4, "foo");
  Token t3(Token::Type::INSTRUCTION, 8, 4, "foo");

  cr_assert_eq(t1, t2);
  cr_assert_neq(t1, t3);
}

Test(assembler, test_token_lt, .init=test_assembler_init, .fini=test_assembler_fini)
{
  Token t1(Token::Type::SYMBOL, 10, 10, "abcdef");
  Token t2(Token::Type::LABEL, 10, 10, "abcdef");
  cr_assert_eq(t1.overlaps(t2), true);
  cr_assert(t2 < t1);
  cr_assert(!(t1 < t2));
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
  src.push_back("main:");
  src.push_back("push psw");
  src.push_back("cpi 8");
  src.push_back("jnz main");

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

    Token(Token::Type::LABEL, 7, 0, "main:"),

    Token(Token::Type::INSTRUCTION, 8, 0, "push"),
    Token(Token::Type::REGISTER, 8, 5, "psw"),

    Token(Token::Type::INSTRUCTION, 9, 0, "cpi"),
    Token(Token::Type::NUMBER, 9, 4, "8"),

    Token(Token::Type::INSTRUCTION, 10, 0, "jnz"),
    Token(Token::Type::SYMBOL, 10, 4, "main"),
  };


  auto tokens = Assembler::tokenize(src);

  cr_assert_eq(tokens.size(), expected_tokens.size());
  for (int i = 0; i < tokens.size(); i++)
  {
    cr_assert_eq(tokens[i], expected_tokens[i]);
  }
}

Test(assembler, test_convert_numbers, .init=test_assembler_init, .fini=test_assembler_fini)
{
  auto tokens = Assembler::tokenize(R"(
    mvi c, 10
    mvi c, 0xa
    mvi c, 0b1010
  )");

  auto converted_tokens = Assembler::convert_numbers(tokens);

  Token tok_dec = tokens[2];
  tok_dec.type = Token::Type::NUMBER;
  tok_dec.value = "10";

  Token tok_hex = tokens[5];
  tok_hex.type = Token::Type::NUMBER;
  tok_hex.value = "10";


  Token tok_bin = tokens[8];
  tok_bin.type = Token::Type::NUMBER;
  tok_bin.value = "10";

  cr_assert_eq(converted_tokens.size(), tokens.size());
  cr_assert_eq(converted_tokens[2], tok_dec);
  cr_assert_eq(converted_tokens[5], tok_hex);
  cr_assert_eq(converted_tokens[8], tok_bin);
}

Test(assembler, test_convert_labels, .init=test_assembler_init, .fini=test_assembler_fini)
{
  std::string source = R"(
    .org 0x100

    main:
      mov c, a
      mvi c, 9

    label:
      jmp label
    
    cpi 0
    jnz main
  )";

  auto tokens = Assembler::tokenize(source);
  tokens = Assembler::convert_numbers(tokens);
  auto converted = Assembler::convert_labels(tokens);
  std::cout << "LABEL: " << converted[15].value << " " << std::to_string(0x100 + 3) << std::endl;
  cr_assert_eq(std::stoul(converted[10].value), 0x103);
  cr_assert_eq(std::stoul(converted[14].value), 0x100);
  // TODO: add test for intermediate .org
}

Test(assembler, test_assemble, .init=test_assembler_init, .fini=test_assembler_fini)
{
  std::string source = R"(
    .org 0x100
    main:
      push psw
     
    mainloop:
      mvi c, 10
      dcr c
      mov a, c
      out 0
      cpi 0
      jnz mainloop
    pop psw
    hlt
  )";

  std::vector<uint8_t> expected_bin = {
    0xf5, // push psw
    0x0e, 0x0a, // mvi c, 10
    0x0d, // dcr c
    0x79, // mov a, c
    0xd3, 0x00, // out 0
    0xfe, 0x00, // cpi 0
    0xc2, 0x01, 0x01, // jnz mainloop
    0xf1, // pop psw
    0x76 // hlt
  };

  auto result_bin = Assembler::assemble(source);

  std::cout << "expected: " << std::endl;
  for (auto byte : expected_bin)
    std::cout << std::hex << std::setw(2) << std::setfill('0') << +byte << " ";
  std::cout << std::endl;

  std::cout << "result: " << std::endl;
  for (auto byte : result_bin)
    std::cout << std::hex << std::setw(2) << std::setfill('0') << +byte << " ";
  std::cout << std::endl;

  cr_assert_eq(expected_bin, result_bin);
}
