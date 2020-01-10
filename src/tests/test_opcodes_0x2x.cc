#include "test_opcodes_0x2x.hh"


extern std::unique_ptr<Emulator> emu;

void test_0x2x_init(void)
{
  emu = std::make_unique<Emulator>();
}

void test_0x2x_fini(void)
{
  emu.reset();
}

Test(opcode, 0x21_lxi_hl, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  auto hl = Rand16();
  emu->load_program({0x21, hl.to8().second, hl.to8().first, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->h, hl.to8().first);
  cr_assert_eq(emu->cpu->l, hl.to8().second);
}

Test(opcode, 0x22_shld, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  auto addr = Rand16();
  auto hl = Rand16();
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->load_program({0x22, addr.to8().second, addr.to8().first, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->ram->memory[addr.val], hl.to8().second);
  cr_assert_eq(emu->cpu->ram->memory[addr.val+1], hl.to8().first);
}

Test(opcode, 0x23_inx_hl, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  auto hl = Rand16();
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  hl.val++;
  emu->execute_opcode(0x23);
  cr_assert_eq(emu->cpu->h, hl.to8().first);
  cr_assert_eq(emu->cpu->l, hl.to8().second);
}

Test(opcode, 0x24_inr_h, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t h = rand8();
  emu->cpu->h = h;
  emu->execute_opcode(0x24);
  cr_assert_eq(emu->cpu->h, ++h);
}

Test(opcode, 0x25_dcr_h, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t h = rand8();
  emu->cpu->h = h;
  emu->execute_opcode(0x25);
  cr_assert_eq(emu->cpu->h, --h);
}

Test(opcode, 0x26_mvi_h, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t val = rand8();
  emu->load_program({0x26, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->h, val);
}

Test(opcode, 0x29_dad_hl, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  auto hl = Rand16();
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->execute_opcode(0x29);
  hl.val += hl.val;
  
  cr_assert_eq(emu->cpu->h, hl.to8().first);
  cr_assert_eq(emu->cpu->l, hl.to8().second);
}

Test(opcode, 0x2a_lhld, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  auto addr = Rand16();
  auto val = Rand16();
  emu->cpu->ram->memory[addr.val] = val.to8().first;
  emu->cpu->ram->memory[addr.val+1] = val.to8().second;
  emu->load_program({0x2a, addr.to8().second, addr.to8().first, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->h, emu->cpu->ram->memory[addr.val+1]);
  cr_assert_eq(emu->cpu->l, emu->cpu->ram->memory[addr.val]);
}

Test(opcode, 0x2b_dcx_hl, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  auto hl = Rand16();
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->execute_opcode(0x2b);
  hl.val--;
  cr_assert_eq(emu->cpu->h, hl.to8().first);
  cr_assert_eq(emu->cpu->l, hl.to8().second);
}

Test(opcode, 0x2c_inr_l, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t l = rand8();
  emu->cpu->l = l;
  emu->execute_opcode(0x2c);
  cr_assert_eq(emu->cpu->l, ++l);
}

Test(opcode, 0x2d_dcr_l, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t l = rand8();
  emu->cpu->l = l;
  emu->execute_opcode(0x2d);
  cr_assert_eq(emu->cpu->l, --l);
}

Test(opcode, 0x2e_mvi_l, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t val = rand8();
  emu->load_program({0x2e, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->l, val);
}

Test(opcode, 0x2f_cma, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t a = rand8();
  emu->cpu->a = a;
  emu->execute_opcode(0x2f);
  a = ~a;
  cr_assert_eq(emu->cpu->a, a);
}
