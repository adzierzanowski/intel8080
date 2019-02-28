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
    std::vector<uint8_t> prog = {
      0x06, // mvi b, 0x05
      0x05,
      0x05, // dcr b
      0xc2, // jnz 0x0102 
      0x02,
      0x01,
      0x00  // term
    };

    cpu.loadProgram(prog, 0x100);
  }

  //cpu.printMemory();
  cpu.setProgramCounter(0x100);
  cpu.execute();

  return 0;
}
