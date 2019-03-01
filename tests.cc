#include "Intel8080.h"

Intel8080 cpu;

void jnzTest(void)
{
  std::vector<uint8_t> prog = {
    0x06, // mvi b, 0x4
    0x04,
    0x05, // dcr b
    0xc2, // jnz 0x0102
    0x02,
    0x01,
    0x0e, // mvi c, 0x00
    0x00,
    0xcd, // call 0x0005
    0x05,
    0x00
  };
  cpu.loadProgram(prog, 0x100);
  cpu.setProgramCounter(0x100);
  cpu.execute();
}

int main()
{
  jnzTest();
}