#include "test_opcodes_mov.hh"


extern std::unique_ptr<Emulator> emu;
extern uint8_t a, b, c, d, e, h, l, m;
extern Rand16 hl;

void test_mov_init()
{
  emu = std::make_unique<Emulator>();
  emu->cpu->a = a = rand8();
  emu->cpu->b = b = rand8();
  emu->cpu->c = c = rand8();
  emu->cpu->d = d = rand8();
  emu->cpu->e = e = rand8();
  emu->cpu->h = h = rand8();
  emu->cpu->l = l = rand8();
  m = rand8();
  hl = Rand16();
}

void test_mov_fini()
{
  emu.reset();
}

Test(opcode, mov_b, .init=test_mov_init, .fini=test_mov_fini)
{
  emu->execute_opcode(0x40);
  cr_assert_eq(emu->cpu->b, b);
  emu->execute_opcode(0x41);
  cr_assert_eq(emu->cpu->b, c);
  emu->execute_opcode(0x42);
  cr_assert_eq(emu->cpu->b, d);
  emu->execute_opcode(0x43);
  cr_assert_eq(emu->cpu->b, e);
  emu->execute_opcode(0x44);
  cr_assert_eq(emu->cpu->b, h);
  emu->execute_opcode(0x45);
  cr_assert_eq(emu->cpu->b, l);

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  emu->execute_opcode(0x46);
  cr_assert_eq(emu->cpu->b, m);
  emu->execute_opcode(0x47);
  cr_assert_eq(emu->cpu->b, a);
}

Test(opcode, mov_c, .init=test_mov_init, .fini=test_mov_fini)
{
  emu->execute_opcode(0x49);
  cr_assert_eq(emu->cpu->c, c);
  emu->execute_opcode(0x48);
  cr_assert_eq(emu->cpu->c, b);
  emu->execute_opcode(0x4a);
  cr_assert_eq(emu->cpu->c, d);
  emu->execute_opcode(0x4b);
  cr_assert_eq(emu->cpu->c, e);
  emu->execute_opcode(0x4c);
  cr_assert_eq(emu->cpu->c, h);
  emu->execute_opcode(0x4d);
  cr_assert_eq(emu->cpu->c, l);

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  emu->execute_opcode(0x4e);
  cr_assert_eq(emu->cpu->c, m);
  emu->execute_opcode(0x4f);
  cr_assert_eq(emu->cpu->c, a);
}

Test(opcode, mov_d, .init=test_mov_init, .fini=test_mov_fini)
{
  emu->execute_opcode(0x52);
  cr_assert_eq(emu->cpu->d, d);
  emu->execute_opcode(0x50);
  cr_assert_eq(emu->cpu->d, b);
  emu->execute_opcode(0x51);
  cr_assert_eq(emu->cpu->d, c);
  emu->execute_opcode(0x53);
  cr_assert_eq(emu->cpu->d, e);
  emu->execute_opcode(0x54);
  cr_assert_eq(emu->cpu->d, h);
  emu->execute_opcode(0x55);
  cr_assert_eq(emu->cpu->d, l);

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  emu->execute_opcode(0x56);
  cr_assert_eq(emu->cpu->d, m);
  emu->execute_opcode(0x57);
  cr_assert_eq(emu->cpu->d, a);
}

Test(opcode, mov_e, .init=test_mov_init, .fini=test_mov_fini)
{
  emu->execute_opcode(0x5b);
  cr_assert_eq(emu->cpu->e, e);
  emu->execute_opcode(0x58);
  cr_assert_eq(emu->cpu->e, b);
  emu->execute_opcode(0x59);
  cr_assert_eq(emu->cpu->e, c);
  emu->execute_opcode(0x5a);
  cr_assert_eq(emu->cpu->e, d);
  emu->execute_opcode(0x5c);
  cr_assert_eq(emu->cpu->e, h);
  emu->execute_opcode(0x5d);
  cr_assert_eq(emu->cpu->e, l);

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  emu->execute_opcode(0x5e);
  cr_assert_eq(emu->cpu->e, m);
  emu->execute_opcode(0x5f);
  cr_assert_eq(emu->cpu->e, a);
}

Test(opcode, mov_h, .init=test_mov_init, .fini=test_mov_fini)
{
  emu->execute_opcode(0x64);
  cr_assert_eq(emu->cpu->h, h);
  emu->execute_opcode(0x60);
  cr_assert_eq(emu->cpu->h, b);
  emu->execute_opcode(0x61);
  cr_assert_eq(emu->cpu->h, c);
  emu->execute_opcode(0x62);
  cr_assert_eq(emu->cpu->h, d);
  emu->execute_opcode(0x63);
  cr_assert_eq(emu->cpu->h, e);
  emu->execute_opcode(0x65);
  cr_assert_eq(emu->cpu->h, l);

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  emu->execute_opcode(0x66);
  cr_assert_eq(emu->cpu->h, m);
  emu->execute_opcode(0x67);
  cr_assert_eq(emu->cpu->h, a);
}

Test(opcode, mov_l, .init=test_mov_init, .fini=test_mov_fini)
{
  emu->execute_opcode(0x6d);
  cr_assert_eq(emu->cpu->l, l);
  emu->execute_opcode(0x68);
  cr_assert_eq(emu->cpu->l, b);
  emu->execute_opcode(0x69);
  cr_assert_eq(emu->cpu->l, c);
  emu->execute_opcode(0x6a);
  cr_assert_eq(emu->cpu->l, d);
  emu->execute_opcode(0x6b);
  cr_assert_eq(emu->cpu->l, e);
  emu->execute_opcode(0x6c);
  cr_assert_eq(emu->cpu->l, h);

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  emu->execute_opcode(0x6e);
  cr_assert_eq(emu->cpu->l, m);
  emu->execute_opcode(0x6f);
  cr_assert_eq(emu->cpu->l, a);
}

Test(opcode, mov_m, .init=test_mov_init, .fini=test_mov_fini)
{
  emu->cpu->h = h = hl.to8().first;
  emu->cpu->l = l = hl.to8().second;

  emu->execute_opcode(0x70);
  cr_assert_eq(emu->cpu->ram->memory[hl.val], b);
  emu->execute_opcode(0x71);
  cr_assert_eq(emu->cpu->ram->memory[hl.val], c);
  emu->execute_opcode(0x72);
  cr_assert_eq(emu->cpu->ram->memory[hl.val], d);
  emu->execute_opcode(0x73);
  cr_assert_eq(emu->cpu->ram->memory[hl.val], e);
  emu->execute_opcode(0x74);
  cr_assert_eq(emu->cpu->ram->memory[hl.val], h);
  emu->execute_opcode(0x75);
  cr_assert_eq(emu->cpu->ram->memory[hl.val], l);
  emu->execute_opcode(0x77);
  cr_assert_eq(emu->cpu->ram->memory[hl.val], a);
}

Test(opcode, mov_a, .init=test_mov_init, .fini=test_mov_fini)
{
  emu->execute_opcode(0x7f);
  cr_assert_eq(emu->cpu->a, a);
  emu->execute_opcode(0x78);
  cr_assert_eq(emu->cpu->a, b);
  emu->execute_opcode(0x79);
  cr_assert_eq(emu->cpu->a, c);
  emu->execute_opcode(0x7a);
  cr_assert_eq(emu->cpu->a, d);
  emu->execute_opcode(0x7b);
  cr_assert_eq(emu->cpu->a, e);
  emu->execute_opcode(0x7c);
  cr_assert_eq(emu->cpu->a, h);
  emu->execute_opcode(0x7d);
  cr_assert_eq(emu->cpu->a, l);

  emu->cpu->h = hl.to8().first;
  emu->cpu->l = hl.to8().second;
  emu->cpu->ram->memory[hl.val] = m;
  emu->execute_opcode(0x7e);
  cr_assert_eq(emu->cpu->a, m);
}
