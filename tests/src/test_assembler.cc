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
  Token t1(Token::Type::IDENTIFIER, 10, 10, "abcdef");
  Token t2(Token::Type::LABEL, 10, 10, "abcdef");
  cr_assert_eq(t1.overlaps(t2), true);
  cr_assert(t2 < t1);
  cr_assert(!(t1 < t2));
}

Test(assembler, test_token_constraint, .init=test_assembler_init, .fini=test_assembler_fini)
{
  Token t1(Token::Type::REGISTER, 10, 10, "b");
  Token t2(Token::Type::NUMBER, 10, 10, "257");
  Token t3(Token::Type::REGISTER, 10, 10, "h");

  cr_assert_eq(t1.constraint_fulfilled(Constraint::ABCDEHLM), true);
  cr_assert_eq(t1.constraint_fulfilled(Constraint::BD), true);
  cr_assert_eq(t1.constraint_fulfilled(Constraint::BDHPSW), true);
  cr_assert_eq(t1.constraint_fulfilled(Constraint::BDHSP), true);
  cr_assert_eq(t1.constraint_fulfilled(Constraint::IMM8), false);
  cr_assert_eq(t1.constraint_fulfilled(Constraint::IMM16), false);
  cr_assert_eq(t1.constraint_fulfilled(Constraint::NONE), true);

  cr_assert_eq(t2.constraint_fulfilled(Constraint::ABCDEHLM), false);
  cr_assert_eq(t2.constraint_fulfilled(Constraint::BD), false);
  cr_assert_eq(t2.constraint_fulfilled(Constraint::BDHPSW), false);
  cr_assert_eq(t2.constraint_fulfilled(Constraint::BDHSP), false);
  cr_assert_eq(t2.constraint_fulfilled(Constraint::IMM8), false);
  cr_assert_eq(t2.constraint_fulfilled(Constraint::IMM16), true);
  cr_assert_eq(t2.constraint_fulfilled(Constraint::NONE), true);

  cr_assert_eq(t3.constraint_fulfilled(Constraint::ABCDEHLM), true);
  cr_assert_eq(t3.constraint_fulfilled(Constraint::BD), false);
  cr_assert_eq(t3.constraint_fulfilled(Constraint::BDHPSW), true);
  cr_assert_eq(t3.constraint_fulfilled(Constraint::BDHSP), true);
  cr_assert_eq(t3.constraint_fulfilled(Constraint::IMM8), false);
  cr_assert_eq(t3.constraint_fulfilled(Constraint::IMM16), false);
  cr_assert_eq(t3.constraint_fulfilled(Constraint::NONE), true);
}

Test(assembler, test_tokenize, .init=test_assembler_init, .fini=test_assembler_fini)
{
  std::string src = R"(
      .org 0x100 .foo .bar 2
    mvi a,     55
    
    mov a, c
    MOV B,C
    hlt lxi sp, 0x44ae
    
    main:
      PUSH PSW
      cpi 8
      jnz main ;this is a comment
    mvi a, 41h
    "string literal"
    'another string literal'
  )";

  std::vector<Token> expected_tokens = {
    Token(Token::Type::DIRECTIVE, 1, 6, ".org"),
    Token(Token::Type::HEXADECIMAL, 1, 11, "0x100"),
    Token(Token::Type::DIRECTIVE, 1, 17, ".foo"),
    Token(Token::Type::DIRECTIVE, 1, 22, ".bar"),
    Token(Token::Type::NUMBER, 1, 27, "2"),

    Token(Token::Type::INSTRUCTION, 2, 4, "mvi"),
    Token(Token::Type::REGISTER, 2, 8, "a"),
    Token(Token::Type::NUMBER, 2, 15, "55"),

    Token(Token::Type::INSTRUCTION, 4, 4, "mov"),
    Token(Token::Type::REGISTER, 4, 8, "a"),
    Token(Token::Type::REGISTER, 4, 11, "c"),

    Token(Token::Type::INSTRUCTION, 5, 4, "mov"),
    Token(Token::Type::REGISTER, 5, 8, "b"),
    Token(Token::Type::REGISTER, 5, 10, "c"),

    Token(Token::Type::INSTRUCTION, 6, 4, "hlt"),
    Token(Token::Type::INSTRUCTION, 6, 8, "lxi"),
    Token(Token::Type::REGISTER, 6, 12, "sp"),
    Token(Token::Type::HEXADECIMAL, 6, 16, "0x44ae"),

    Token(Token::Type::LABEL, 8, 4, "main:"),

    Token(Token::Type::INSTRUCTION, 9, 6, "push"),
    Token(Token::Type::REGISTER, 9, 11, "psw"),

    Token(Token::Type::INSTRUCTION, 10, 6, "cpi"),
    Token(Token::Type::NUMBER, 10, 10, "8"),

    Token(Token::Type::INSTRUCTION, 11, 6, "jnz"),
    Token(Token::Type::IDENTIFIER, 11, 10, "main"),
    Token(Token::Type::COMMENT, 11, 15, ";this is a comment"),

    Token(Token::Type::INSTRUCTION, 12, 4, "mvi"),
    Token(Token::Type::REGISTER, 12, 8, "a"),
    Token(Token::Type::HEXADECIMAL, 12, 11, "41h"),

    Token(Token::Type::STRING, 13, 4, R"("string literal")"),
    Token(Token::Type::STRING, 14, 4, "'another string literal'"),
  };


  auto tokens = Assembler::tokenize(src);

  for (auto tok : tokens)
    std::cout << tok << std::endl;

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
    mvi c, ah
    mvi c, 0b1010
  )");

  auto converted_tokens = Assembler::convert_numbers(tokens);

  Token tok_dec = tokens[2];
  tok_dec.type = Token::Type::NUMBER;
  tok_dec.value = "10";

  Token tok_hex = tokens[5];
  tok_hex.type = Token::Type::NUMBER;
  tok_hex.value = "10";

  Token tok_hex2 = tokens[8];
  tok_hex2.type = Token::Type::NUMBER;
  tok_hex2.value = "10";

  Token tok_bin = tokens[11];
  tok_bin.type = Token::Type::NUMBER;
  tok_bin.value = "10";

  cr_assert_eq(converted_tokens.size(), tokens.size());
  cr_assert_eq(converted_tokens[2], tok_dec);
  cr_assert_eq(converted_tokens[5], tok_hex);
  cr_assert_eq(converted_tokens[8], tok_hex2);
  cr_assert_eq(converted_tokens[11], tok_bin);
}

Test(assembler, test_convert_labels, .init=test_assembler_init, .fini=test_assembler_fini)
{
  std::string source = R"(
    .org 0x100

    main:
      mov c, a
      mvi c, 9
      ; this is a comment, does it break the test?

    label:
      jmp label
    
    cpi 0
    jnz main
  )";

  auto tokens = Assembler::tokenize(source);
  tokens = Assembler::convert_numbers(tokens);
  auto converted = Assembler::convert_labels(tokens);
  cr_assert_eq(std::stoul(converted[11].value), 0x103);
  cr_assert_eq(std::stoul(converted[15].value), 0x100);
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

    op_8x:
      add b
      add c
      add d
      add e
      add h
      add l
      add m
      add a
      adc b
      adc c
      adc d
      adc e
      adc h
      adc l
      adc m
      adc a

    op_9x:
      sub b
      sub c
      sub d
      sub e
      sub h
      sub l
      sub m
      sub a
      sbb b
      sbb c
      sbb d
      sbb e
      sbb h
      sbb l
      sbb m
      sbb a

    op_ax:
      ana b
      ana c
      ana d
      ana e
      ana h
      ana l
      ana m
      ana a
      xra b
      xra c
      xra d
      xra e
      xra h
      xra l
      xra m
      xra a

    op_bx:
      ora b
      ora c
      ora d
      ora e
      ora h
      ora l
      ora m
      ora a
      cmp b
      cmp c
      cmp d
      cmp e
      cmp h
      cmp l
      cmp m
      cmp a
    
    op_cx:
      rnz
      pop b
      jnz 0xdead
      jmp 0xbeef
      cnz 0xdead
      push b
      adi 0xca
      rst 0
      rz
      ret
      jz 0xbeef
      cz 0xdead
      call 0xbeef
      aci 0xfe
      rst 1

    op_dx:
      rnc
      pop d
      jnc 0xdead
      out 0xca
      cnc 0xdead
      push d
      sui 0xfe
      rst 2
      rc
      jc 0xbeef
      in 0xca
      cc 0xbeef
      sbi 0xfe
      rst 3
    
    op_ex:
      rpo
      pop h
      jpo 0xdead
      xthl
      cpo 0xbeef
      push h
      ani 0xca
      rst 4
      rpe
      pchl
      jpe 0xdead
      xchg
      cpe 0xbeef
      xri 0xfe
      rst 5

    op_fx:
      rp
      pop psw
      jp 0xdead
      di
      cp 0xbeef
      push psw
      ori 0xca
      rst 6
      rm
      sphl
      jm 0xdead
      ei
      cm 0xbeef
      cpi 0xfe
      rst 7
  )";

  std::vector<uint8_t> expected_bin = {
    0x00, 0x01, 0xad, 0xde, 0x02, 0x03, 0x04, 0x05, 0x06, 0xbe, 0x07,
    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0xef, 0x0f,

    0x11, 0xad, 0xde, 0x12, 0x13, 0x14, 0x15, 0x16, 0xbe, 0x17,
    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0xef, 0x1f,

    0x21, 0xad, 0xde, 0x22, 0xef, 0xbe, 0x23, 0x24, 0x25, 0x26, 0xbe, 0x27,
    0x29, 0x2a, 0xad, 0xde, 0x2b, 0x2c, 0x2d, 0x2e, 0xef, 0x2f,

    0x31, 0xad, 0xde, 0x32, 0xef, 0xbe, 0x33, 0x34, 0x35, 0x36, 0xbe, 0x37,
    0x39, 0x3a, 0xad, 0xde, 0x3b, 0x3c, 0x3d, 0x3e, 0xef, 0x3f,

    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,

    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,

    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,

    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,

    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,

    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,

    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,

    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,

    0xc0, 0xc1, 0xc2, 0xad, 0xde, 0xc3, 0xef, 0xbe, 0xc4, 0xad, 0xde, 0xc5, 0xc6, 0xca, 0xc7,
    0xc8, 0xc9, 0xca, 0xef, 0xbe, 0xcc, 0xad, 0xde, 0xcd, 0xef, 0xbe, 0xce, 0xfe, 0xcf,

    0xd0, 0xd1, 0xd2, 0xad, 0xde, 0xd3, 0xca, 0xd4, 0xad, 0xde, 0xd5, 0xd6, 0xfe, 0xd7,
    0xd8, 0xda, 0xef, 0xbe, 0xdb, 0xca, 0xdc, 0xef, 0xbe, 0xde, 0xfe, 0xdf,

    0xe0, 0xe1, 0xe2, 0xad, 0xde, 0xe3, 0xe4, 0xef, 0xbe, 0xe5, 0xe6, 0xca, 0xe7,
    0xe8, 0xe9, 0xea, 0xad, 0xde, 0xeb, 0xec, 0xef, 0xbe, 0xee, 0xfe, 0xef,

    0xf0, 0xf1, 0xf2, 0xad, 0xde, 0xf3, 0xf4, 0xef, 0xbe, 0xf5, 0xf6, 0xca, 0xf7,
    0xf8, 0xf9, 0xfa, 0xad, 0xde, 0xfb, 0xfc, 0xef, 0xbe, 0xfe, 0xfe, 0xff
  };

  auto result_bin = Assembler::assemble(source);
  cr_assert_eq(expected_bin, result_bin);
}
