#include "test.hh"

std::unique_ptr<Emulator> emu;
uint8_t a, b, c, d, e, h, l;
uint8_t m;
Rand16 hl, val;
uint16_t sp;
bool zero_flag;

int randint(int min, int max)
{
  std::mt19937 gen(std::random_device{}());
  return std::uniform_int_distribution<int>{min, max}(gen);
}

uint8_t rand8(void)
{
  return randint(0x00, 0xff);
}
