#include "test_opcodes_0x8x.hh"


extern std::unique_ptr<Emulator> emu;
extern uint8_t a;

void test_0x8x_init(void)
{
  emu = std::make_unique<Emulator>();
  a = rand8();
  emu->cpu->a = a;
}

void test_0x8x_fini(void)
{
  emu.reset();
}

Test(opcode, add_b, .init=test_0x8x_init, .fini=test_0x8x_fini)
{
  uint8_t b = rand8();
  emu->cpu->b = b;
  emu->execute_opcode(0x80);
  a += b;
  cr_assert_eq(emu->cpu->a, a);
}

Test(opcode, add_c, .init=test_0x8x_init, .fini=test_0x8x_fini)
{
  uint8_t c = rand8();
  emu->cpu->c = c;
  emu->execute_opcode(0x81);
  a += c;
  cr_assert_eq(emu->cpu->a, a);
}

Test(opcode, add_d, .init=test_0x8x_init, .fini=test_0x8x_fini)
{
  uint8_t d = rand8();
  emu->cpu->d = d;
  emu->execute_opcode(0x82);
  a += d;
  cr_assert_eq(emu->cpu->a, a);
}

Test(opcode, add_e, .init=test_0x8x_init, .fini=test_0x8x_fini)
{
  uint8_t e = rand8();
  emu->cpu->e = e;
  emu->execute_opcode(0x83);
  a += e;
  cr_assert_eq(emu->cpu->a, a);
}

Test(opcode, add_h, .init=test_0x8x_init, .fini=test_0x8x_fini)
{
  uint8_t h = rand8();
  emu->cpu->h = h;
  emu->execute_opcode(0x84);
  a += h;
  cr_assert_eq(emu->cpu->a, a);
}

Test(opcode, add_l, .init=test_0x8x_init, .fini=test_0x8x_fini)
{
  uint8_t l = rand8();
  emu->cpu->l = l;
  emu->execute_opcode(0x85);
  a += l;
  cr_assert_eq(emu->cpu->a, a);
}

Test(opcode, add_m, .init=test_0x8x_init, .fini=test_0x8x_fini)
{
  uint8_t m = rand8();
  auto hl = Rand16();
  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  emu->execute_opcode(0x86);
  a += m;
  cr_assert_eq(emu->cpu->a, a);
}

Test(opcode, add_a, .init=test_0x8x_init, .fini=test_0x8x_fini)
{
  emu->execute_opcode(0x87);
  a += a;
  cr_assert_eq(emu->cpu->a, a);
}
