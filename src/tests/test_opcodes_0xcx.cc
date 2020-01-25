#include "test_opcodes_0xcx.hh"


extern std::unique_ptr<Emulator> emu;

void test_0xcx_init(void)
{
  emu = std::make_unique<Emulator>();
}

void test_0xcx_fini(void)
{
  emu.reset();
}

Test(opcode, 0xc1_pop_bc, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  uint8_t b = rand8();
  uint8_t c = rand8();
  auto sp = Rand16();
  if (sp.val < 0xff)
    sp.val += 0xff;
  else if (sp.val > 0xff00)
    sp.val -= 0xff;
  auto val = Rand16();
  emu->cpu->b = b;
  emu->cpu->c = c;
  emu->cpu->sp = sp.val;
  emu->cpu->ram->memory[sp.val] = val.to8().second;
  emu->cpu->ram->memory[sp.val+1] = val.to8().first;

  emu->execute_opcode(0xc1);
  cr_assert_eq(emu->cpu->b, val.to8().first);
  cr_assert_eq(emu->cpu->c, val.to8().second);
  cr_assert_eq(emu->cpu->sp, sp.val + 2);
}

Test(opcode, 0xc5_push_bc, .init=test_0xcx_init, .fini=test_0xcx_fini)
{
  uint8_t b = rand8();
  uint8_t c = rand8();
  auto sp = Rand16();
  if (sp.val < 0xff)
    sp.val += 0xff;
  else if (sp.val > 0xff00)
    sp.val -= 0xff;
  auto val = Rand16();
  emu->cpu->b = b;
  emu->cpu->c = c;
  emu->cpu->sp = sp.val;
  emu->cpu->ram->memory[sp.val] = val.to8().second;
  emu->cpu->ram->memory[sp.val+1] = val.to8().first;

  emu->execute_opcode(0xc5);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 1 + 2], b);
  cr_assert_eq(emu->cpu->ram->memory[emu->cpu->sp - 2 + 2], c);
  cr_assert_eq(emu->cpu->sp, sp.val - 2);
}
