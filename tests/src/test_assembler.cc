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

Test(assembler, test_assemble_all, .init=test_assembler_init, .fini=test_assembler_fini)
{
  std::string source = R"(
    .org 0x100
    op_0x:
      nop
      lxi b, 0xdead
      stax b
      inx b
      inr b
      dcr b
      mvi b, 0xbe
      rlc
      dad b
      ldax b
      dcx b
      inr c
      dcr c
      mvi c, 0xef
      rrc

    op_1x:
      lxi d, 0xdead
      stax d
      inx d
      inr d
      dcr d
      mvi d, 0xbe
      ral
      dad d
      ldax d
      dcx d
      inr e
      dcr e
      mvi e, 0xef
      rar

    op_2x:
      lxi h, 0xdead
      shld, 0xbeef
      inx h
      inr h
      dcr h
      mvi h, 0xbe
      daa
      dad h
      lhld 0xdead
      dcx h
      inr l
      dcr l
      mvi l, 0xef
      cma

    op_3x:
      lxi sp, 0xdead
      sta, 0xbeef
      inx sp
      inr m
      dcr m
      mvi m, 0xbe
      stc
      dad sp
      lda 0xdead
      dcx sp
      inr a
      dcr a
      mvi a, 0xef
      cmc

    op_4x:
      mov b, b
      mov b, c
      mov b, d
      mov b, e
      mov b, h
      mov b, l
      mov b, m
      mov b, a
      mov c, b
      mov c, c
      mov c, d
      mov c, e
      mov c, h
      mov c, l
      mov c, m
      mov c, a

    op_5x:
      mov d, b
      mov d, c
      mov d, d
      mov d, e
      mov d, h
      mov d, l
      mov d, m
      mov d, a
      mov e, b
      mov e, c
      mov e, d
      mov e, e
      mov e, h
      mov e, l
      mov e, m
      mov e, a

    op_6x:
      mov h, b
      mov h, c
      mov h, d
      mov h, e
      mov h, h
      mov h, l
      mov h, m
      mov h, a
      mov l, b
      mov l, c
      mov l, d
      mov l, e
      mov l, h
      mov l, l
      mov l, m
      mov l, a

    op_7x:
      mov m, b
      mov m, c
      mov m, d
      mov m, e
      mov m, h
      mov m, l
      hlt
      mov m, a
      mov a, b
      mov a, c
      mov a, d
      mov a, e
      mov a, h
      mov a, l
      mov a, m
      mov a, a
  )";

  std::vector<uint8_t> expected_bin = {
    0x00, 0x01, 0xad, 0xde, 0x02, 0x03, 0x04, 0x05, 0x06, 0xbe, 0x07, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0xef, 0x0f,
    0x11, 0xad, 0xde, 0x12, 0x13, 0x14, 0x15, 0x16, 0xbe, 0x17, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0xef, 0x1f,
    0x21, 0xad, 0xde, 0x22, 0xef, 0xbe, 0x23, 0x24, 0x25, 0x26, 0xbe, 0x27, 0x29, 0x2a, 0xad, 0xde, 0x2b, 0x2c, 0x2d, 0x2e, 0xef, 0x2f,
    0x31, 0xad, 0xde, 0x32, 0xef, 0xbe, 0x33, 0x34, 0x35, 0x36, 0xbe, 0x37, 0x39, 0x3a, 0xad, 0xde, 0x3b, 0x3c, 0x3d, 0x3e, 0xef, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f
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
