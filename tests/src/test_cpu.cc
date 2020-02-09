#include "test_cpu.hh"


void test_cpu_init(void)
{
  cpu = std::make_unique<CPU>();
}

void test_cpu_fini(void)
{
  cpu.reset();
}

Test(cpu, cpu_initialization, .init=test_cpu_init, .fini=test_cpu_fini)
{
  cr_assert_eq(cpu->a, 0);
  cr_assert_eq(cpu->b, 0);
  cr_assert_eq(cpu->c, 0);
  cr_assert_eq(cpu->d, 0);
  cr_assert_eq(cpu->e, 0);
  cr_assert_eq(cpu->h, 0);
  cr_assert_eq(cpu->l, 0);
  cr_assert_eq(cpu->flags, 0b00000010);
  cr_assert_eq(cpu->pc, 0);
}

Test(cpu, load_program, .init=test_cpu_init, .fini=test_cpu_fini)
{
  cpu->load_program({0x76}, 0x100);
  cr_assert_eq(cpu->ram->memory[0x100], 0x76);
  cr_assert_eq(cpu->pc, 0x100);
}

Test(cpu, set_flag, .init=test_cpu_init, .fini=test_cpu_fini)
{
  cpu->set_flag(Flag::S, true);
  cr_assert_eq(cpu->flags, 0b10000010);
  cpu->set_flag(Flag::S, false);
  cr_assert_eq(cpu->flags, 0b00000010);

  cpu->set_flag(Flag::Z, true);
  cr_assert_eq(cpu->flags, 0b01000010);
  cpu->set_flag(Flag::Z, false);
  cr_assert_eq(cpu->flags, 0b00000010);

  cpu->set_flag(Flag::Z1, true);
  cr_assert_eq(cpu->flags, 0b00000010);
  cpu->set_flag(Flag::Z1, false);
  cr_assert_eq(cpu->flags, 0b00000010);

  cpu->set_flag(Flag::AC, true);
  cr_assert_eq(cpu->flags, 0b00010010);
  cpu->set_flag(Flag::AC, false);
  cr_assert_eq(cpu->flags, 0b00000010);

  cpu->set_flag(Flag::Z2, true);
  cr_assert_eq(cpu->flags, 0b00000010);
  cpu->set_flag(Flag::Z2, false);
  cr_assert_eq(cpu->flags, 0b00000010);

  cpu->set_flag(Flag::P, true);
  cr_assert_eq(cpu->flags, 0b00000110);
  cpu->set_flag(Flag::P, false);
  cr_assert_eq(cpu->flags, 0b00000010);

  cr_assert_eq(cpu->flags, 0b00000010);
  cpu->set_flag(Flag::O, false);
  cr_assert_eq(cpu->flags, 0b00000010);

  cpu->set_flag(Flag::C, true);
  cr_assert_eq(cpu->flags, 0b00000011);
  cpu->set_flag(Flag::C, false);
  cr_assert_eq(cpu->flags, 0b00000010);
}

Test(cpu, get_flag, .init=test_cpu_init, .fini=test_cpu_fini)
{
  cpu->flags = 0b11010111;
  cr_assert_eq(cpu->get_flag(Flag::S), true);
  cr_assert_eq(cpu->get_flag(Flag::Z), true);
  cr_assert_eq(cpu->get_flag(Flag::Z1), false);
  cr_assert_eq(cpu->get_flag(Flag::AC), true);
  cr_assert_eq(cpu->get_flag(Flag::Z2), false);
  cr_assert_eq(cpu->get_flag(Flag::P), true);
  cr_assert_eq(cpu->get_flag(Flag::O), true);
  cr_assert_eq(cpu->get_flag(Flag::C), true);
}
