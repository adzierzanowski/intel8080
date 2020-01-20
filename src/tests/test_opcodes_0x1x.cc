#include "test_opcodes_0x1x.hh"


extern std::unique_ptr<Emulator> emu;

void test_0x1x_init(void)
{
  emu = std::make_unique<Emulator>();
}

void test_0x1x_fini(void)
{
  emu.reset();
}

Test(opcode, 0x11_lxi_de, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  auto de = Rand16();
  emu->load_program({0x11, de.to8().second, de.to8().first, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->d, de.to8().first);
  cr_assert_eq(emu->cpu->e, de.to8().second);
}

Test(opcode, 0x12_stax_de, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  auto addr = Rand16();
  uint8_t a = rand8();
  emu->cpu->a = a;
  emu->cpu->d = addr.to8().first;
  emu->cpu->e = addr.to8().second;
  emu->execute_opcode(0x12);
  cr_assert_eq(emu->cpu->ram->memory[addr.val], a);
}

Test(opcode, 0x13_inx_de, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  auto de = Rand16();
  emu->cpu->d = de.to8().first;
  emu->cpu->e = de.to8().second;
  de.val++;
  emu->execute_opcode(0x13);
  cr_assert_eq(emu->cpu->d, de.to8().first);
  cr_assert_eq(emu->cpu->e, de.to8().second);
}

Test(opcode, 0x14_inr_d, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  uint8_t d = rand8();
  emu->cpu->d = d;
  uint8_t res = d + 1;
  emu->execute_opcode(0x14);
  cr_assert_eq(emu->cpu->d, res);
  test_flags_szap(d, res);
}

Test(opcode, 0x15_dcr_d, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  uint8_t d = rand8();
  emu->cpu->d = d;
  uint8_t res = d - 1;
  emu->execute_opcode(0x15);
  cr_assert_eq(emu->cpu->d, res);
  test_flags_szap(d, res);
}

Test(opcode, 0x16_mvi_d, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  uint8_t val = rand8();
  emu->load_program({0x16, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->d, val);
}

Test(opcode, 0x17_ral, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  emu->cpu->set_flag(Flag::C, (bool) randint(0, 1));
  uint8_t a = rand8();
  uint8_t carry = emu->cpu->get_flag(Flag::C) ? 1 : 0;
  uint8_t hb = (a & 0x80) >> 7;
  emu->cpu->a = a;
  uint8_t res = (a << 1) | carry;
  emu->execute_opcode(0x17);
  cr_assert_eq(emu->cpu->a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), hb);
}

Test(opcode, 0x19_dad_d, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  auto de = Rand16();
  auto hl = Rand16();
  emu->cpu->d = de.to8().first;
  emu->cpu->e = de.to8().second;
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->execute_opcode(0x19);
  uint16_t res = hl.val + de.val;
  
  cr_assert_eq(emu->cpu->h, (res & 0xff00) >> 8);
  cr_assert_eq(emu->cpu->l, res & 0x00ff);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < hl.val || res < de.val);
}

Test(opcode, 0x1a_ldax_d, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  auto addr = Rand16();
  uint8_t val = rand8();
  emu->cpu->ram->memory[addr.val] = val;
  emu->cpu->a = rand8();
  emu->cpu->d = addr.to8().first;
  emu->cpu->e = addr.to8().second;
  emu->execute_opcode(0x1a);
  cr_assert_eq(emu->cpu->a, val);
}

Test(opcode, 0x1b_dcx_d, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  auto de = Rand16();
  emu->cpu->d = de.to8().first;
  emu->cpu->e = de.to8().second;
  emu->execute_opcode(0x1b);
  de.val--;
  cr_assert_eq(emu->cpu->d, de.to8().first);
  cr_assert_eq(emu->cpu->e, de.to8().second);
}

Test(opcode, 0x1c_inr_e, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  uint8_t e = rand8();
  emu->cpu->e = e;
  uint8_t res = e + 1;
  emu->execute_opcode(0x1c);
  cr_assert_eq(emu->cpu->e, res);
  test_flags_szap(e, res);
}

Test(opcode, 0x1d_dcr_e, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  uint8_t e = rand8();
  emu->cpu->e = e;
  uint8_t res = e - 1;
  emu->execute_opcode(0x1d);
  cr_assert_eq(emu->cpu->e, res);
  test_flags_szap(e, res);
}

Test(opcode, 0x1e_mvi_e, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  uint8_t val = rand8();
  emu->load_program({0x1e, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->e, val);
}

Test(opcode, 0x1f_rar, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  emu->cpu->set_flag(Flag::C, (bool) randint(0, 1));
  uint8_t a = rand8();
  uint8_t lb = a & 1;
  uint8_t carry = emu->cpu->get_flag(Flag::C) ? 1 : 0;
  uint8_t res = (a >> 1) | (carry << 7);
  emu->cpu->a = a;
  emu->execute_opcode(0x1f);
  cr_assert_eq(emu->cpu->a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), lb);
}
