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
  emu->execute_opcode(0x14);
  cr_assert_eq(emu->cpu->d, ++d);
}

Test(opcode, 0x15_dcr_d, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  uint8_t d = rand8();
  emu->cpu->d = d;
  emu->execute_opcode(0x15);
  cr_assert_eq(emu->cpu->d, --d);
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
  uint8_t a = rand8();
  emu->cpu->a = a;
  emu->execute_opcode(0x17);
  a <<= 1;
  cr_assert_eq(emu->cpu->a, a);
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
  hl.val += de.val;
  
  cr_assert_eq(emu->cpu->h, hl.to8().first);
  cr_assert_eq(emu->cpu->l, hl.to8().second);
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
  emu->execute_opcode(0x1c);
  cr_assert_eq(emu->cpu->e, ++e);
}

Test(opcode, 0x1d_dcr_e, .init=test_0x1x_init, .fini=test_0x1x_fini)
{
  uint8_t e = rand8();
  emu->cpu->e = e;
  emu->execute_opcode(0x1d);
  cr_assert_eq(emu->cpu->e, --e);
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
  uint8_t a = rand8();
  emu->cpu->a = a;
  emu->execute_opcode(0x1f);
  a >>= 1;
  cr_assert_eq(emu->cpu->a, a);
}
