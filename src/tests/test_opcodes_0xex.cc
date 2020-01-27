#include "test_opcodes_0xex.hh"


extern std::unique_ptr<Emulator> emu;
extern Rand16 val;
extern uint16_t sp;
extern bool parity_flag;

void test_0xex_init(void)
{
  emu = std::make_unique<Emulator>();

  val = Rand16();
  sp = 0x100;
  parity_flag = (bool) randint(0, 1);
  emu->cpu->set_flag(Flag::P, parity_flag);
}

void test_0xex_fini(void)
{
  emu.reset();
}

Test(opcode, 0xe0_rpo, .init=test_0xex_init, .fini=test_0xex_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xe0, 0x76, 0x76});
  emu->execute();

  if (parity_flag)
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

Test(opcode, 0xe1_pop_hl, .init=test_0xex_init, .fini=test_0xex_fini)
{
  uint8_t h = rand8();
  uint8_t l = rand8();
  emu->cpu->h = h;
  emu->cpu->l = l;
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;

  emu->execute_opcode(0xe1);
  cr_assert_eq(emu->cpu->h, val.to8().first);
  cr_assert_eq(emu->cpu->l, val.to8().second);
  cr_assert_eq(emu->cpu->sp, sp + 2);
}

Test(opcode, 0xe2_jpo, .init=test_0xex_init, .fini=test_0xex_fini)
{
  emu->load_program({0xe2, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();

  if (parity_flag)
  {
    cr_assert_eq(emu->cpu->pc, 4);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 6);
  }
}

Test(opcode, 0xe3_xthl, .init=test_0xex_init, .fini=test_0xex_fini)
{
  uint8_t h = rand8();
  uint8_t l = rand8();
  emu->cpu->sp = sp;
  emu->cpu->h = h;
  emu->cpu->l = l;
  emu->cpu->ram->memory[sp] = val.to8().first;
  emu->cpu->ram->memory[sp+1] = val.to8().second;
  
  emu->execute_opcode(0xe3);
  cr_assert_eq(emu->cpu->h, val.to8().second, "got: %d exp: %d", emu->cpu->h, val.to8().second);
  cr_assert_eq(emu->cpu->l, val.to8().first);
  cr_assert_eq(emu->cpu->ram->memory[sp], l);
  cr_assert_eq(emu->cpu->ram->memory[sp+1], h);
}

Test(opcode, 0xe4_cpo, .init=test_0xex_init, .fini=test_0xex_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xe4, 0x04, 0x00, // 0x0000 cnc 0x0004
    0x76,             // 0x0003 hlt
    0x00,             // 0x0004 nop
    0x76              // 0x0005 hlt
  });
  emu->execute();

  if (parity_flag)
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

Test(opcode, 0xe5_push_hl, .init=test_0xex_init, .fini=test_0xex_fini)
{
  uint8_t h = rand8();
  uint8_t l = rand8();
  emu->cpu->h = h;
  emu->cpu->l = l;
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;

  emu->execute_opcode(0xe5);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 1 + 2], h);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 2 + 2], l);
  cr_assert_eq(emu->cpu->sp, sp - 2);
}

Test(opcode, 0xe6_ani, .init=test_0xex_init, .fini=test_0xex_fini)
{
  uint8_t a = rand8();
  uint8_t val = rand8();
  uint8_t res = a & val;
  emu->cpu->a = a;

  emu->load_program({0xe6, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
}

Test(opcode, 0xe7_rst_4, .init=test_0xex_init, .fini=test_0xex_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;
  emu->cpu->pc = 0x1000;

  emu->execute_opcode(0xe7);
  cr_assert_eq(emu->cpu->sp, sp-2);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp], 0x01);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp+1], 0x10);
  cr_assert_eq(emu->cpu->pc, 32);
}

Test(opcode, 0xe8_rpe, .init=test_0xex_init, .fini=test_0xex_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xe8, 0x76, 0x76});
  emu->execute();

  if (parity_flag)
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


Test(opcode, 0xe9_pchl, .init=test_0xex_init, .fini=test_0xex_fini)
{
  uint8_t h = rand8();
  uint8_t l = rand8();
  emu->cpu->h = h;
  emu->cpu->l = l;
  emu->execute_opcode(0xe9);
  cr_assert_eq(emu->cpu->pc, (h << 8) | l);
}

Test(opcode, 0xea_jpe, .init=test_0xex_init, .fini=test_0xex_fini)
{
  emu->load_program({0xea, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();

  if (parity_flag)
  {
    cr_assert_eq(emu->cpu->pc, 6);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 4);
  }
}

Test(opcode, 0xeb_xchg, .init=test_0xex_init, .fini=test_0xex_fini)
{
  uint8_t h = rand8();
  uint8_t l = rand8();
  uint8_t d = rand8();
  uint8_t e = rand8();
  emu->cpu->h = h;
  emu->cpu->l = l;
  emu->cpu->d = d;
  emu->cpu->e = e;

  emu->execute_opcode(0xeb);
  cr_assert_eq(emu->cpu->h, d);
  cr_assert_eq(emu->cpu->l, e);
  cr_assert_eq(emu->cpu->d, h);
  cr_assert_eq(emu->cpu->e, l);
}

Test(opcode, 0xec_cpe, .init=test_0xex_init, .fini=test_0xex_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xec, 0x04, 0x00, // 0x0000 cz 0x0004
    0x76,             // 0x0003 hlt
    0x00,             // 0x0004 nop
    0x76              // 0x0005 hlt
  });
  emu->execute();

  if (parity_flag)
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

Test(opcode, 0xed_call, .init=test_0xex_init, .fini=test_0xex_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xed, 0x04, 0x00, // 0x0000 call 0x0004
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

Test(opcode, 0xee_xri, .init=test_0xex_init, .fini=test_0xex_fini)
{
  uint8_t a = rand8();
  uint8_t val = rand8();
  uint8_t res = a ^ val;
  emu->cpu->a = a;

  emu->load_program({0xee, val, 0x76});
  emu->execute();

  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res > a);
}

Test(opcode, 0xef_rst_5, .init=test_0xex_init, .fini=test_0xex_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;
  emu->cpu->pc = 0x1000;

  emu->execute_opcode(0xef);
  cr_assert_eq(emu->cpu->sp, sp-2);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp], 0x01);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp+1], 0x10);
  cr_assert_eq(emu->cpu->pc, 40);
}
