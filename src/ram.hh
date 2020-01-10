#ifndef RAM_H
#define RAM_H

#include <cstdint>
#include <vector>


class RAM
{
  private:
    std::vector<uint8_t> memory;
  
  public:
    RAM(void);
    size_t get_size(void);
    void set_values(std::vector<uint8_t> values, uint16_t offset);
    uint8_t get(uint16_t ptr);
    void set(uint16_t ptr, uint8_t value);
};

#endif
