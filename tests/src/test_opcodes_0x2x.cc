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
  uint8_t res = h + 1;
  emu->execute_opcode(0x24);
  cr_assert_eq(emu->cpu->h, res);
  test_flags_szap(h, res);
}

Test(opcode, 0x25_dcr_h, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t h = rand8();
  emu->cpu->h = h;
  uint8_t res = h - 1;
  emu->execute_opcode(0x25);
  cr_assert_eq(emu->cpu->h, h - 1);
  test_flags_szap(h, res);
}

Test(opcode, 0x26_mvi_h, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t val = rand8();
  emu->load_program({0x26, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->h, val);
}

Test(opcode, 0x27_daa, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t a = rand8();
  bool c = (bool) randint(0, 1);
  bool ac = (bool) randint(0, 1);
  emu->cpu->a = a;
  emu->cpu->set_flag(Flag::C, c);
  emu->cpu->set_flag(Flag::AC, ac);

  bool lb_condition = ((a & 0x0f) > 9) || ac;
  uint8_t lb_adjusted = lb_condition ? a+6 : a;
  uint8_t hb = (a & 0xf0) >> 4;
  bool hb_condition = (hb > 9) || c;
  uint8_t hb_adjusted = hb_condition ? lb_adjusted | ((hb+9) << 4) : lb_adjusted;


  cr_assert_eq(emu->cpu->a, hb_adjusted);
  test_flags_szap(a, hb_adjusted);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), hb_adjusted > a);
}

Test(opcode, 0x29_dad_hl, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  auto hl = Rand16();
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->execute_opcode(0x29);
  uint16_t res = hl.val + hl.val;
  
  cr_assert_eq(emu->cpu->h, (res & 0xff00) >> 8);
  cr_assert_eq(emu->cpu->l, res & 0x00ff);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < hl.val);
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
  uint8_t res = l + 1;
  emu->execute_opcode(0x2c);
  cr_assert_eq(emu->cpu->l, res);
  test_flags_szap(l, res);
}

Test(opcode, 0x2d_dcr_l, .init=test_0x2x_init, .fini=test_0x2x_fini)
{
  uint8_t l = rand8();
  emu->cpu->l = l;
  uint8_t res = l - 1;
  emu->execute_opcode(0x2d);
  cr_assert_eq(emu->cpu->l, res);
  test_flags_szap(l, res);
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
