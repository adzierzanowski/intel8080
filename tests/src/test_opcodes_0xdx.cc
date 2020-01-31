#include "test_opcodes_0xdx.hh"


extern std::unique_ptr<Emulator> emu;
extern Rand16 val;
extern uint16_t sp;
extern bool carry_flag;

void test_0xdx_init(void)
{
  emu = std::make_unique<Emulator>();

  val = Rand16();
  sp = 0x100;
  carry_flag = (bool) randint(0, 1);
  emu->cpu->set_flag(Flag::C, carry_flag);
}

void test_0xdx_fini(void)
{
  emu.reset();
}

Test(opcode, 0xd0_rnc, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xd0, 0x76, 0x76});
  emu->execute();

  if (carry_flag)
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

Test(opcode, 0xd1_pop_de, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  uint8_t d = rand8();
  uint8_t e = rand8();
  emu->cpu->d = d;
  emu->cpu->e = e;
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;

  emu->execute_opcode(0xd1);
  cr_assert_eq(emu->cpu->d, val.to8().first);
  cr_assert_eq(emu->cpu->e, val.to8().second);
  cr_assert_eq(emu->cpu->sp, sp + 2);
}

Test(opcode, 0xd2_jnc, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->load_program({0xd2, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();

  if (carry_flag)
  {
    cr_assert_eq(emu->cpu->pc, 4);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 6);
  }
}

/* TODO:
Test(opcode, 0xd3_out, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
}
*/

Test(opcode, 0xd4_cnc, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xd4, 0x04, 0x00, // 0x0000 cnc 0x0004
    0x76,             // 0x0003 hlt
    0x00,             // 0x0004 nop
    0x76              // 0x0005 hlt
  });
  emu->execute();

  if (carry_flag)
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

Test(opcode, 0xd5_push_de, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  uint8_t d = rand8();
  uint8_t e = rand8();
  emu->cpu->d = d;
  emu->cpu->e = e;
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;

  emu->execute_opcode(0xd5);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 1 + 2], d);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 2 + 2], e);
  cr_assert_eq(emu->cpu->sp, sp - 2);
}

Test(opcode, 0xd6_sui, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  uint8_t a = rand8();
  uint8_t val = rand8();
  uint8_t res = a - val;
  emu->cpu->a = a;

  emu->load_program({0xd6, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
}

Test(opcode, 0xd7_rst_2, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;
  emu->cpu->pc = 0x1000;

  emu->execute_opcode(0xd7);
  cr_assert_eq(emu->cpu->sp, sp-2);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp], 0x01);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp+1], 0x10);
  cr_assert_eq(emu->cpu->pc, 16);
}

Test(opcode, 0xd8_rc, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xd8, 0x76, 0x76});
  emu->execute();

  if (carry_flag)
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


Test(opcode, 0xd9_ret, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xd9, 0x76, 0x76});
  emu->execute();

  cr_assert_eq(emu->cpu->pc, 3);
  cr_assert_eq(emu->cpu->sp, sp+2);
}

Test(opcode, 0xda_jc, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->load_program({0xda, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();

  if (carry_flag)
  {
    cr_assert_eq(emu->cpu->pc, 6);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 4);
  }
}

/* TODO:
Test(opcode, 0xdb_in, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
}
*/

Test(opcode, 0xdc_cc, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xdc, 0x04, 0x00, // 0x0000 cz 0x0004
    0x76,             // 0x0003 hlt
    0x00,             // 0x0004 nop
    0x76              // 0x0005 hlt
  });
  emu->execute();

  if (carry_flag)
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

Test(opcode, 0xdd_call, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xdd, 0x04, 0x00, // 0x0000 call 0x0004
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

Test(opcode, 0xde_sbi, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  uint8_t a = rand8();
  uint8_t val = rand8();
  uint8_t res = a - val - (uint8_t) carry_flag;
  emu->cpu->a = a;

  emu->load_program({0xde, val, 0x76});
  emu->execute();

  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
}

Test(opcode, 0xdf_rst_3, .init=test_0xdx_init, .fini=test_0xdx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;
  emu->cpu->pc = 0x1000;

  emu->execute_opcode(0xdf);
  cr_assert_eq(emu->cpu->sp, sp-2);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp], 0x01);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp+1], 0x10);
  cr_assert_eq(emu->cpu->pc, 24);
}
