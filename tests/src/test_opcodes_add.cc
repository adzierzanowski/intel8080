#include "test_opcodes_add.hh"


extern std::unique_ptr<Emulator> emu;
extern uint8_t a, b, c, d, e, h, l, m;
extern Rand16 hl;

void test_add_init()
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

void test_add_fini()
{
  emu.reset();
}

Test(opcode, add, .init=test_add_init, .fini=test_add_fini)
{
  uint8_t res;

  res = a + b;
  emu->execute_opcode(0x80);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += b;

  res = a + c;
  emu->execute_opcode(0x81);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += c;

  res = a + d;
  emu->execute_opcode(0x82);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += d;

  res = a + e;
  emu->execute_opcode(0x83);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += e;

  res = a + h;
  emu->execute_opcode(0x84);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += h;

  res = a + l;
  emu->execute_opcode(0x85);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += l;

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  res = a + m;
  emu->execute_opcode(0x86);
  cr_assert_eq(emu->cpu->a, res, "Got: %d Expected: %d", emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += m;

  res = a + a;
  emu->execute_opcode(0x87);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
}

Test(opcode, adc, .init=test_add_init, .fini=test_add_fini)
{
  uint8_t res, carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a + b + carry;
  emu->execute_opcode(0x88);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += b + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a + c + carry;
  emu->execute_opcode(0x89);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += c + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a + d + carry;
  emu->execute_opcode(0x8a);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += d + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a + e + carry;
  emu->execute_opcode(0x8b);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += e + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a + h + carry;
  emu->execute_opcode(0x8c);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += h + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a + l + carry;
  emu->execute_opcode(0x8d);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += l + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  res = a + m + carry;
  emu->execute_opcode(0x8e);
  cr_assert_eq(emu->cpu->a, res, "Got: %d Expected: %d", emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
  a += m + carry;

  carry = randint(0, 1);
  emu->cpu->set_flag(Flag::C, (bool) carry);
  res = a + a + carry;
  emu->execute_opcode(0x8f);
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
}
