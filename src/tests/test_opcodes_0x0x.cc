#include "test_opcodes_0x0x.hh"


extern std::unique_ptr<Emulator> emu;

void test_0x0x_init(void)
{
  emu = std::make_unique<Emulator>();
}

void test_0x0x_fini(void)
{
  emu.reset();
}

Test(opcode, 0x00_nop, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  cr_assert_eq(emu->cpu->flags, 0b00000010);
}

Test(opcode, 0x01_lxi_bc, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  auto bc = Rand16();
  emu->load_program({0x01, bc.to8().second, bc.to8().first, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->b, bc.to8().first);
  cr_assert_eq(emu->cpu->c, bc.to8().second);
  cr_assert_eq(emu->cpu->flags, 0b00000010);
}

Test(opcode, 0x02_stax_b, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  auto addr = Rand16();
  uint8_t a = rand8();
  emu->cpu->a = a;
  emu->cpu->b = addr.to8().first;
  emu->cpu->c = addr.to8().second;
  emu->execute_opcode(0x02);
  cr_assert_eq(emu->cpu->ram->memory[addr.val], a);
  cr_assert_eq(emu->cpu->flags, 0b00000010);
}

Test(opcode, 0x03_inx_bc, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  auto bc = Rand16();
  emu->cpu->b = bc.to8().first;
  emu->cpu->c = bc.to8().second;
  bc.val++;
  emu->execute_opcode(0x03);
  cr_assert_eq(emu->cpu->b, bc.to8().first);
  cr_assert_eq(emu->cpu->c, bc.to8().second);
  cr_assert_eq(emu->cpu->flags, 0b00000010);
}

Test(opcode, 0x04_inr_b, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  uint8_t b = rand8();
  uint8_t res = b+1;
  emu->cpu->b = b;
  emu->execute_opcode(0x04);
  cr_assert_eq(emu->cpu->b, res);

  cr_assert_eq(emu->cpu->get_flag(Flag::S), res >= 0b01111111);
  cr_assert_eq(emu->cpu->get_flag(Flag::Z), res == 0);
  cr_assert_eq(emu->cpu->get_flag(Flag::AC), aux_carry(b, res));
  cr_assert_eq(emu->cpu->get_flag(Flag::P), even_parity(res));
}

Test(opcode, 0x05_dcr_b, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  uint8_t b = rand8();
  emu->cpu->b = b;
  emu->execute_opcode(0x05);
  cr_assert_eq(emu->cpu->b, --b);
}

Test(opcode, 0x06_mvi_b, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  uint8_t val = rand8();
  emu->load_program({0x06, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->b, val);
}

Test(opcode, 0x07_rlc, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  uint8_t a = rand8();
  emu->cpu->a = a;
  a <<= 1;
  emu->execute_opcode(0x07);
  cr_assert_eq(emu->cpu->a, a);
}

Test(opcode, 0x09_dad_b, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  auto bc = Rand16();
  auto hl = Rand16();
  emu->cpu->b = bc.to8().first;
  emu->cpu->c = bc.to8().second;
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->execute_opcode(0x09);
  hl.val += bc.val;
  
  cr_assert_eq(emu->cpu->h, hl.to8().first);
  cr_assert_eq(emu->cpu->l, hl.to8().second);
}

Test(opcode, 0x0a_ldax_b, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  auto addr = Rand16();
  uint8_t val = rand8();
  emu->cpu->ram->memory[addr.val] = val;
  emu->cpu->a = rand8();
  emu->cpu->b = addr.to8().first;
  emu->cpu->c = addr.to8().second;
  emu->execute_opcode(0x0a);
  cr_assert_eq(emu->cpu->a, val);
}

Test(opcode, 0x0b_dcx_b, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  auto bc = Rand16();
  emu->cpu->b = bc.to8().first;
  emu->cpu->c = bc.to8().second;
  emu->execute_opcode(0x0b);
  bc.val--;
  cr_assert_eq(emu->cpu->b, bc.to8().first);
  cr_assert_eq(emu->cpu->c, bc.to8().second);
}

Test(opcode, 0x0c_inr_c, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  uint8_t c = rand8();
  emu->cpu->c = c;
  emu->execute_opcode(0x0c);
  cr_assert_eq(emu->cpu->c, ++c);
}

Test(opcode, 0x0d_dcr_c, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  uint8_t c = rand8();
  emu->cpu->c = c;
  emu->execute_opcode(0x0d);
  cr_assert_eq(emu->cpu->c, --c);
}

Test(opcode, 0x0e_mvi_c, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  uint8_t val = rand8();
  emu->load_program({0x0e, val, 0x76});
  emu->execute();
  cr_assert_eq(emu->cpu->c, val);
}

Test(opcode, 0x0f_rrc, .init=test_0x0x_init, .fini=test_0x0x_fini)
{
  uint8_t a = rand8();
  emu->cpu->a = a;
  a >>= 1;
  emu->execute_opcode(0x0f);
  cr_assert_eq(emu->cpu->a, a);
}
