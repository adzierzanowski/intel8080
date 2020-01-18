#include "test.hh"

std::unique_ptr<Emulator> emu;
uint8_t a, b, c, d, e, h, l;
uint8_t m;
Rand16 hl;

int randint(int min, int max)
{
  std::mt19937 gen(std::random_device{}());
  return std::uniform_int_distribution<int>{min, max}(gen);
}

uint8_t rand8(void)
{
  return randint(0x00, 0xff);
}

bool even_parity(uint8_t val)
{
  int acc = 0;
  for (int i = 0; i < 8; i++)
  {
    acc += val & 1;
    val >>= 1;
  }
  return acc % 2 == 0;
}
