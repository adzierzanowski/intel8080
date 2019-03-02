#include "Intel8080.h"

int main(int argc, char *argv[])
{
  Intel8080 cpu;
  cpu.setProgramCounter(0x100);

  if (argc > 1)
  {
    cpu.loadProgram(std::string(argv[1]), 0x100);
    if (argc > 2 && argv[2] == std::string("-q"))
      cpu.debugOutput = false;
  }

  else
  {
    std::vector<uint8_t> helloWorld = {
      0x0e, // mvi c, 0x09
      0x09,
      0x11, // lxi D:E, 0x0112
      0x12,
      0x01,
      0xcd, // call 0x0005
      0x05,
      0x00,
      0x0e, // mvi c, 0xff
      0xff,
      0xcd, // call 0x0005
      0x05,
      0x00,
      0x0e, // mvi c, 0x00
      0x00,
      0xcd, // call 0x0005
      0x05,
      0x00,
      0x48, // db "Hello, world!\n$"
      0x65,
      0x6c,
      0x6c,
      0x6f,
      0x2c,
      0x20,
      0x77,
      0x6f,
      0x72,
      0x6c,
      0x64,
      0x21,
      0x0a,
      0x24
    };

    cpu.debugOutput = false;
    cpu.loadProgram(helloWorld, 0x100);
  }

  cpu.execute();

  return 0;
}
