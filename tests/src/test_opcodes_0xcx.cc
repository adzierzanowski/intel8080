#include "test_opcodes_0xcx.hh"


extern std::unique_ptr<Emulator> emu;
extern Rand16 val;
extern uint16_t sp;
extern bool zero_flag;

void test_0xcx_init(void)
{
  emu = std::make_unique<Emulator>();

  val = Rand16();
  sp = 0x100;
  zero_flag = (bool) randint(0, 1);
  emu->cpu->set_flag(Flag::Z, zero_flag);
}

void test_0xcx_fini(void)
{
  emu.reset();
}

Test(opcode, 0xc0_rnz, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xc0, 0x76, 0x76});
  emu->execute();

  if (zero_flag)
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

Test(opcode, 0xc1_pop_bc, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  uint8_t b = rand8();
  uint8_t c = rand8();
  emu->cpu->b = b;
  emu->cpu->c = c;
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;

  emu->execute_opcode(0xc1);
  cr_assert_eq(emu->cpu->b, val.to8().first);
  cr_assert_eq(emu->cpu->c, val.to8().second);
  cr_assert_eq(emu->cpu->sp, sp + 2);
}

Test(opcode, 0xc2_jnz, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->load_program({0xc2, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();

  if (zero_flag)
  {
    cr_assert_eq(emu->cpu->pc, 4);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 6);
  }
}

Test(opcode, 0xc3_jmp, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->load_program({0xc3, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->pc, 6, "Got: %d Exp: %d", emu->cpu->pc, 6);
}

Test(opcode, 0xc4_cnz, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xc4, 0x04, 0x00, // 0x0000 cnz 0x0004
    0x76,             // 0x0003 hlt
    0x00,             // 0x0004 nop
    0x76              // 0x0005 hlt
  });
  emu->execute();

  if (zero_flag)
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

Test(opcode, 0xc5_push_bc, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  uint8_t b = rand8();
  uint8_t c = rand8();
  emu->cpu->b = b;
  emu->cpu->c = c;
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;

  emu->execute_opcode(0xc5);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 1 + 2], b);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 2 + 2], c);
  cr_assert_eq(emu->cpu->sp, sp - 2);
}

Test(opcode, 0xc6_adi, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  uint8_t a = rand8();
  uint8_t val = rand8();
  uint8_t res = a + val;
  emu->cpu->a = a;

  emu->load_program({0xc6, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
}

Test(opcode, 0xc7_rst_0, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;
  emu->cpu->pc = 0x1000;

  emu->execute_opcode(0xc7);
  cr_assert_eq(emu->cpu->sp, sp-2);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp], 0x01);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp+1], 0x10);
  cr_assert_eq(emu->cpu->pc, 0);
}

Test(opcode, 0xc8_rz, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xc8, 0x76, 0x76});
  emu->execute();

  if (zero_flag)
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


Test(opcode, 0xc9_ret, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = 0x02;
  emu->cpu->ram->memory[sp+1] = 0x00;

  emu->load_program({0xc9, 0x76, 0x76});
  emu->execute();

  cr_assert_eq(emu->cpu->pc, 3);
  cr_assert_eq(emu->cpu->sp, sp+2);
}

Test(opcode, 0xca_jz, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->load_program({0xca, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();

  if (zero_flag)
  {
    cr_assert_eq(emu->cpu->pc, 6);
  }
  else
  {
    cr_assert_eq(emu->cpu->pc, 4);
  }
}

Test(opcode, 0xcb_jmp, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->load_program({0xcb, 0x05, 0x00, 0x76, 0x00, 0x76});
  emu->execute();

  cr_assert_eq(emu->cpu->pc, 6);
}

Test(opcode, 0xcc_cz, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->cpu->set_flag(Flag::Z, zero_flag);
  emu->cpu->sp = sp;

  emu->load_program({
    0xcc, 0x04, 0x00, // 0x0000 cz 0x0004
    0x76,             // 0x0003 hlt
    0x00,             // 0x0004 nop
    0x76              // 0x0005 hlt
  });
  emu->execute();

  if (zero_flag)
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

Test(opcode, 0xcd_call, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->cpu->sp = sp;

  emu->load_program({
    0xcd, 0x04, 0x00, // 0x0000 call 0x0004
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

Test(opcode, 0xce_aci, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  uint8_t carry = randint(0, 1);
  uint8_t a = rand8();
  uint8_t val = rand8();
  uint8_t res = a + val + carry;
  emu->cpu->a = a;
  emu->cpu->set_flag(Flag::C, (bool) carry);

  emu->load_program({0xce, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->a, res);
  test_flags_szap(a, res);
  cr_assert_eq(emu->cpu->get_flag(Flag::C), res < a);
}

Test(opcode, 0xcf_rst_1, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  emu->cpu->sp = sp;
  emu->cpu->ram->memory[sp] = val.to8().second;
  emu->cpu->ram->memory[sp+1] = val.to8().first;
  emu->cpu->pc = 0x1000;

  emu->execute_opcode(0xcf);
  cr_assert_eq(emu->cpu->sp, sp-2);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp], 0x01);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp+1], 0x10);
  cr_assert_eq(emu->cpu->pc, 8);
}
