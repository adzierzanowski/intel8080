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
      0x08, // nop
      0x06, // mvi b, 0x22
      0x22,
      0x04, // inr b
      0x10, // dump
      0x41, // mov b, c
      0x10, // dump
      0x55, // mov d, l
      0x10, // dump
      0x00  // term
    };

    cpu.loadProgram(prog, 0x100);
  }

  //cpu.printMemory();
  cpu.setProgramCounter(0x100);
  cpu.execute();

  return 0;
}
