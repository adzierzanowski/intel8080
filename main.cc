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

    std::vector<uint8_t> calltest = {
      0x0e, // mvi c, 2
      0x02,
      0x1e, // mvi e, 'a'
      0x61,
      0xcd, // call 0x0005
      0x05,
      0x00,
      0x1e, // mvi e, '\n'
      0x0a,
      0xcd, // call 0x0005
      0x05,
      0x00,
      0x00 // term
    };

    std::vector<uint8_t> writestrtest = {
      0x0e, // mvi c, 9
      0x09,
      0x16, // mvi d, 0x01
      0x01,
      0x1e, // mvi e, 0x0a
      0x0a,
      0xcd, // call 0x0005
      0x05,
      0x00,
      0x00, // term
      0x61, // a
      0x62, // b
      0x63, // c
      0x64, // d
      0x0a, // LF
      0x24  // $
    };

    cpu.loadProgram(writestrtest, 0x100);
  }

  //cpu.printMemory();
  cpu.setProgramCounter(0x100);
  cpu.debugOutput = true;
  cpu.execute();

  return 0;
}
