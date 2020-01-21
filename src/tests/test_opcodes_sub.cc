#include "test_opcodes_sub.hh"


extern std::unique_ptr<Emulator> emu;
extern uint8_t a, b, c, d, e, h, l, m;
extern Rand16 hl;

void test_sub_init()
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

void test_sub_fini()
{
  emu.reset();
}

Test(opcode, sub, .init=test_sub_init, .fini=test_sub_fini)
{
  uint8_t res;

  res = a - b;
  emu->execute_opcode(0x90);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= b;

  res = a - c;
  emu->execute_opcode(0x91);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= c;

  res = a - d;
  emu->execute_opcode(0x92);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= d;

  res = a - e;
  emu->execute_opcode(0x93);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= e;

  res = a - h;
  emu->execute_opcode(0x94);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= h;

  res = a - l;
  emu->execute_opcode(0x95);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= l;

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  res = a - m;
  emu->execute_opcode(0x96);
  cr_assert_eq(emu->cpu->a, res, "Got: %d Expected: %d", emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= m;

  res = a - a;
  emu->execute_opcode(0x97);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
}

Test(opcode, sbb, .init=test_sub_init, .fini=test_sub_fini)
{
  uint8_t res, carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a - b - carry;
  emu->execute_opcode(0x98);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= b + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a - c - carry;
  emu->execute_opcode(0x99);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= c + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a - d - carry;
  emu->execute_opcode(0x9a);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= d + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a - e - carry;
  emu->execute_opcode(0x9b);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= e + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a - h - carry;
  emu->execute_opcode(0x9c);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= h + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a - l - carry;
  emu->execute_opcode(0x9d);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= l + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  res = a - m - carry;
  emu->execute_opcode(0x9e);
  cr_assert_eq(emu->cpu->a, res, "Got: %d Expected: %d", emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
  a -= m + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a - a - carry;
  emu->execute_opcode(0x9f);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
}
