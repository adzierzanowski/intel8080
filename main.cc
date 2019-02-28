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
    std::vector<uint8_t> prog = { 0x08, 0x06, 0x22, 0x10, 0x41, 0x10, 0x55, 0x10, 0x00 };
    cpu.loadProgram(prog, 0x100);
  }

  //cpu.printMemory();
  cpu.setProgramCounter(0x100);
  cpu.execute();

  return 0;
}
