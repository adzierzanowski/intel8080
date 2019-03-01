#include "Intel8080.h"

int main(int argc, char *argv[])
{
  Intel8080 cpu;

  if (argc > 1)
  {
    // load program from file
  }

  else
  {
    std::vector<uint8_t> jmptest = {
      0x06, // mvi b, 0x05
      0x05,
      0x05, // dcr b
      0xc2, // jnz 0x0102 
      0x02,
      0x01,
      0x00  // term
    };

    std::vector<uint8_t> xchgtest = {
      0x16, // mvi d, 0xdd
      0xdd,
      0x1e, // mvi e, 0xee
      0xee,
      0x10, // dump
      0xeb, // xchg
      0x10, // dump
      0x00
    };

    cpu.loadProgram(xchgtest, 0x100);
  }

  //cpu.printMemory();
  cpu.setProgramCounter(0x100);
  cpu.execute();

  return 0;
}
