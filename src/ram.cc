#include "ram.hh"


RAM::RAM(void) : memory{std::vector<uint8_t>(0x10000, 0)} {}

size_t RAM::get_size(void)
{
  return memory.size();
}

void RAM::set_values(std::vector<uint8_t> values, uint16_t offset)
{
  std::copy(values.begin(), values.end(), memory.begin() + offset);
}

uint8_t RAM::get(uint16_t ptr)
{
  return memory[ptr];
}

void RAM::set(uint16_t ptr, uint8_t value)
{
  memory[ptr] = value;
}
