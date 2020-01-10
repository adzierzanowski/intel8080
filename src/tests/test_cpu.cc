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
}
