#include "test_opcodes_0xfx.hh"


extern std::unique_ptr<Emulator> emu;
extern Rand16 val;
extern uint16_t sp;
extern bool sign_flag;

void test_0xfx_init(void)
{
  emu = std::make_unique<Emulator>();

  val = Rand16();
  sp = 0x100;
  sign_flag = (bool) randint(0, 1);
  emu->cpu->set_flag(Flag::S, sign_flag);
}

void test_0xfx_fini(void)
{
  emu.reset();
}

Test(opcode, 0xe0_rp, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xf0, 0x76, 0x76});
  emu->execute();

  if (sign_flag)
  {
    cr_assert_eq(emu->cpu->pc, 2);
    cr_assert_eq(emu->cpu->sp, sp);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 3);
    cr_assert_eq(emu->cpu->sp, sp+2);
  }
}

Test(opcode, 0xf1_pop_psw, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  uint8_t a = rand8();
  uint8_t flags = rand8();
  emu->cpu->a = a;
  emu->cpu->flags = flags;
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;

  emu->execute_opcode(0xf1);
  cr_assert_eq(emu->cpu->a, val.to8().first);
  cr_assert_eq(emu->cpu->flags, val.to8().second);
  cr_assert_eq(emu->cpu->sp, sp + 2);
}

Test(opcode, 0xf2_jp, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  emu->load_program({0xf2, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();

  if (sign_flag)
  {
    cr_assert_eq(emu->cpu->pc, 4);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 6);
  }
}

/* TODO:
Test(opcode, 0xf3_di, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
}
*/

Test(opcode, 0xf4_cp, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xf4, 0x04, 0x00, // 0x0000 cnc 0x0004
    0x76,             // 0x0003 hlt
    0x00,             // 0x0004 nop
    0x76              // 0x0005 hlt
  });
  emu->execute();

  if (sign_flag)
  {
    cr_assert_eq(emu->cpu->pc, 0x0004);
    cr_assert_eq(emu->cpu->sp, sp);
    cr_assert_eq(emu->cpu->load(emu->cpu->sp), 0x0000);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 0x0006);
    cr_assert_eq(emu->cpu->sp, sp-2);
    cr_assert_eq(emu->cpu->load(emu->cpu->sp), 0x0003);
  }
}

Test(opcode, 0xf5_push_psw, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  uint8_t a = rand8();
  uint8_t flags = rand8();
  emu->cpu->a = a;
  emu->cpu->flags = flags;
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;

  emu->execute_opcode(0xf5);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 1 + 2], a);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 2 + 2], flags);
  cr_assert_eq(emu->cpu->sp, sp - 2);
}

Test(opcode, 0xf6_ori, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  uint8_t a = rand8();
  uint8_t val = rand8();
  uint8_t res = a | val;
  emu->cpu->a = a;

  emu->load_program({0xf6, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
}

Test(opcode, 0xf7_rst_6, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;
  emu->cpu->pc = 0x1000;

  emu->execute_opcode(0xf7);
  cr_assert_eq(emu->cpu->sp, sp-2);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp], 0x01);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp+1], 0x10);
  cr_assert_eq(emu->cpu->pc, 48);
}

Test(opcode, 0xf8_rm, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xf8, 0x76, 0x76});
  emu->execute();

  if (sign_flag)
  {
    cr_assert_eq(emu->cpu->pc, 3);
    cr_assert_eq(emu->cpu->sp, sp+2);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 2);
    cr_assert_eq(emu->cpu->sp, sp);
  }
}


Test(opcode, 0xf9_sphl, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  uint8_t h = rand8();
  uint8_t l = rand8();
  emu->cpu->h = h;
  emu->cpu->l = l;

  emu->execute_opcode(0xf9);
  cr_assert_eq(emu->cpu->sp, (h << 8) | l);
}

Test(opcode, 0xfa_jm, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  emu->load_program({0xfa, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();

  if (sign_flag)
  {
    cr_assert_eq(emu->cpu->pc, 6);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 4);
  }
}

/* TODO:
Test(opcode, 0xfb_ei, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
}
*/

Test(opcode, 0xfc_cm, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xfc, 0x04, 0x00, // 0x0000 cz 0x0004
    0x76,             // 0x0003 hlt
    0x00,             // 0x0004 nop
    0x76              // 0x0005 hlt
  });
  emu->execute();

  if (sign_flag)
  {
    cr_assert_eq(emu->cpu->pc, 0x0006);
    cr_assert_eq(emu->cpu->sp, sp-2);
    cr_assert_eq(emu->cpu->load(emu->cpu->sp), 0x0003);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 0x0004);
    cr_assert_eq(emu->cpu->sp, sp);
    cr_assert_eq(emu->cpu->load(emu->cpu->sp), 0x0000);
  }
}

Test(opcode, 0xfd_call, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xfd, 0x04, 0x00, // 0x0000 call 0x0004
    0x76,             // 0x0003 hlt
    0x00,             // 0x0004 nop
    0x76              // 0x0005 hlt
  });
  emu->execute();

  cr_assert_eq(emu->cpu->pc, 0x0006);
  cr_assert_eq(emu->cpu->sp, sp-2);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp], 0x03);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp+1], 0x00);
}

Test(opcode, 0xfe_cpi, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  uint8_t a = rand8();
  uint8_t val = rand8();
  uint8_t res = a - val;
  emu->cpu->a = a;

  emu->load_program({0xfe, val, 0x76});
  emu->execute();

  cr_assert_eq(emu->cpu->a, a);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
}

Test(opcode, 0xff_rst_7, .init=test_0xfx_init, .fini=test_0xfx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;
  emu->cpu->pc = 0x1000;

  emu->execute_opcode(0xff);
  cr_assert_eq(emu->cpu->sp, sp-2);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp], 0x01);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp+1], 0x10);
  cr_assert_eq(emu->cpu->pc, 56);
}
