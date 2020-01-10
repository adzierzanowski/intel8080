#ifndef TEST_RAM_H
#define TEST_RAM_H

#include <criterion/criterion.h>


Test(ram, ram_initial_size)
{
  cr_assert_eq(ram.get_size(), 0x1000);
}

Test(ram, set_values)
{
  ram.set_values({1, 2, 3}, 0x100);
  cr_assert_eq(ram.get(0x100), 1);
  cr_assert_eq(ram.get(0x101), 2);
  cr_assert_eq(ram.get(0x102), 3);
}

#endif
