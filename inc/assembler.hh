#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <cstdint>
#include <vector>


class Assembler
{
  std::vector<std::string> source;

  public:
    Assembler();
    std::vector<uint8_t> assemble(void);
};

#endif
