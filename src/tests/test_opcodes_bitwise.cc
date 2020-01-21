#include "test_opcodes_bitwise.hh"


extern std::unique_ptr<Emulator> emu;
extern uint8_t a, b, c, d, e, h, l, m;
extern Rand16 hl;

void test_bitwise_init()
{
  emu = std::make_unique<Emulator>();
  emu->cpu->a = a = rand8();
  emu->cpu->b = b = rand8();
  emu->cpu->c = c = rand8();
  emu->cpu->d = d = rand8();
  emu->cpu->e = e = rand8();
  emu->cpu->h = h = rand8();
  emu->cpu->l = l = rand8();
  m = rand8();
  hl = Rand16();
}

void test_bitwise_fini()
{
  emu.reset();
}

Test(opcode, and, .init=test_bitwise_init, .fini=test_bitwise_fini)
{
  uint8_t res;

  res = a & b;
  emu->execute_opcode(0xa0);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a &= b;

  res = a & c;
  emu->execute_opcode(0xa1);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a &= c;

  res = a & d;
  emu->execute_opcode(0xa2);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a &= d;

  res = a & e;
  emu->execute_opcode(0xa3);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a &= e;

  res = a & h;
  emu->execute_opcode(0xa4);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a &= h;

  res = a & l;
  emu->execute_opcode(0xa5);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a &= l;

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  res = a & m;
  emu->execute_opcode(0xa6);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a &= m;

  res = a & a;
  emu->execute_opcode(0xa7);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
}

Test(opcode, xri, .init=test_bitwise_init, .fini=test_bitwise_fini)
{
  uint8_t res;

  res = a ^ b;
  emu->execute_opcode(0xa8);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a ^= b;

  res = a ^ c;
  emu->execute_opcode(0xa9);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a ^= c;

  res = a ^ d;
  emu->execute_opcode(0xaa);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a ^= d;

  res = a ^ e;
  emu->execute_opcode(0xab);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a ^= e;

  res = a ^ h;
  emu->execute_opcode(0xac);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a ^= h;

  res = a ^ l;
  emu->execute_opcode(0xad);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a ^= l;

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  res = a ^ m;
  emu->execute_opcode(0xae);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a ^= m;

  res = a ^ a;
  emu->execute_opcode(0xaf);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
}

Test(opcode, ora, .init=test_bitwise_init, .fini=test_bitwise_fini)
{
  uint8_t res;

  res = a | b;
  emu->execute_opcode(0xb0);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a |= b;

  res = a | c;
  emu->execute_opcode(0xb1);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a |= c;

  res = a | d;
  emu->execute_opcode(0xb2);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a |= d;

  res = a | e;
  emu->execute_opcode(0xb3);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a |= e;

  res = a | h;
  emu->execute_opcode(0xb4);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a |= h;

  res = a | l;
  emu->execute_opcode(0xb5);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a |= l;

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  res = a | m;
  emu->execute_opcode(0xb6);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
  a |= m;

  res = a | a;
  emu->execute_opcode(0xb7);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), false);
}
