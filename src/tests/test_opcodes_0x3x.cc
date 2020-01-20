#include "test_opcodes_0x3x.hh"


extern std::unique_ptr<Emulator> emu;

void test_0x3x_init(void)
{
  emu = std::make_unique<Emulator>();
}

void test_0x3x_fini(void)
{
  emu.reset();
}

Test(opcode, 0x31_lxi_sp, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  auto sp = Rand16();
  auto imm = Rand16();
  emu->cpu->sp = sp.val;
  emu->load_program({0x31, imm.to8().second, imm.to8().first, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->sp, imm.val);
}

Test(opcode, 0x32_sta, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  uint8_t a = rand8();
  auto addr = Rand16();
  emu->cpu->a = a;
  emu->load_program({0x32, addr.to8().second, addr.to8().first, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->ram->memory[addr.val], a);
}

Test(opcode, 0x33_inx_sp, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  auto sp = Rand16();
  emu->cpu->sp = sp.val;
  emu->execute_opcode(0x33);
  cr_assert_eq(emu->cpu->sp, ++sp.val);
}

Test(opcode, 0x34_inr_m, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  auto hl = Rand16();
  uint8_t val = rand8();
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = val;
  emu->execute_opcode(0x34);
  cr_assert_eq(emu->cpu->ram->memory[hl.val], ++val);
}

Test(opcode, 0x35_dcr_m, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  auto hl = Rand16();
  uint8_t val = rand8();
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = val;
  emu->execute_opcode(0x35);
  cr_assert_eq(emu->cpu->ram->memory[hl.val], --val);
}

Test(opcode, 0x36_mvi_m, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  auto hl = Rand16();
  uint8_t val = rand8();
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->load_program({0x36, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->ram->memory[hl.val], val);
}

Test(opcode, 0x37_stc, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  // TODO:
}

Test(opcode, 0x39_dad_sp, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  auto sp = Rand16();
  auto hl = Rand16();
  emu->cpu->sp = sp.val;
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->execute_opcode(0x39);
  hl.val += sp.val;
  
  cr_assert_eq(emu->cpu->h, hl.to8().first);
  cr_assert_eq(emu->cpu->l, hl.to8().second);
}

Test(opcode, 0x32_lda, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  uint8_t val = rand8();
  auto addr = Rand16();
  emu->cpu->ram->memory[addr.val] = val;
  emu->load_program({0x3a, addr.to8().second, addr.to8().first, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->a, val);
}

Test(opcode, 0x3b_dcx_sp, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  auto sp = Rand16();
  emu->cpu->sp = sp.val;
  emu->execute_opcode(0x3b);
  sp.val--;
  cr_assert_eq(emu->cpu->sp, sp.val);
}

Test(opcode, 0x3c_inr_a, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  uint8_t a = rand8();
  emu->cpu->a = a;
  emu->execute_opcode(0x3c);
  cr_assert_eq(emu->cpu->a, ++a);
}

Test(opcode, 0x3d_dcr_a, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  uint8_t a = rand8();
  emu->cpu->a = a;
  emu->execute_opcode(0x3d);
  cr_assert_eq(emu->cpu->a, --a);
}

Test(opcode, 0x3e_mvi_a, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  uint8_t val = rand8();
  emu->load_program({0x3e, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->a, val);
}

Test(opcode, 0x3f_cmc_a, .init=test_0x3x_init, .fini=test_0x3x_fini)
{
  // TODO:
}
