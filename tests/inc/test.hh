#ifndef TEST_H
#define TEST_H

#include <iomanip>
#include <random>
#include <utility>
#include <criterion/criterion.h>

#define private public

#include "emu.hh"

#define test_flags_szap(b, a) do { \
  cr_assert_eq(emu->cpu->get_flag(Flag::S), a >= 0x7f); \
  cr_assert_eq(emu->cpu->get_flag(Flag::Z), a == 0); \
  cr_assert_eq(emu->cpu->get_flag(Flag::AC), CPU::aux_carry(b, a)); \
  cr_assert_eq(emu->cpu->get_flag(Flag::P), CPU::even_parity(a)); \
} while (0);

int randint(int min, int max);
uint8_t rand8(void);

struct Rand16
{
  uint16_t val;

  Rand16() { 
    val = randint(0x00, 0xffff);
  };

  std::pair<uint8_t, uint8_t> to8(void)
  {
    return std::make_pair<uint8_t, uint8_t>(val >> 8, val & 0xff);
  }
};

#endif
