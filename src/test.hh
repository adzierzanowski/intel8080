#ifndef TEST_H
#define TEST_H

#include <random>
#include <utility>
#include <criterion/criterion.h>

#define private public

#include "emu.hh"



int randint(int min, int max);
uint8_t rand8(void);
void exec_op1(uint8_t op);

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
