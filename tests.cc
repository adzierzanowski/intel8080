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

void printTest(void)
{
  std::vector<uint8_t> prog = {
    0x0e, // mvi c, 0x09
    0x09,
    0x11, // lxi D:E, 0x010d
    0x0d,
    0x01,
    0xcd, // call 0x0005
    0x05,
    0x00,
    0x0e, // mvi c, 0x00,
    0x00,
    0xcd, // call 0x0005
    0x05,
    0x00,
    0x48,
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

  cpu.loadProgram(prog, 0x100);
  cpu.setProgramCounter(0x100);
  cpu.execute();
}

void stackTest(void)
{
  std::vector<uint8_t> prog = {
    // set stack pointer
    0x31, // lxi SP, 0x1000
    0x00,
    0x10,

    // test push and pop
    0x01, // lxi B:C, 0xabcd
    0xcd,
    0xab,
    0xc5, // push B:C
    0xd1, // pop D:E

    // print registers
    0x0e, // mvi c, 0xff
    0xff,
    0xcd, // call 0x0005
    0x05,
    0x00,

    // terminate 
    0x0e, // mvi c, 0x00,
    0x00,
    0xcd, // call 0x0005
    0x05,
    0x00,
  };

  cpu.loadProgram(prog, 0x100);
  cpu.setProgramCounter(0x100);
  cpu.execute();
}

void jmpTest(void)
{
  std::vector<uint8_t> prog = {
    0x00,
    0x00,
    0x00,
    0x00,

    0xc3, // jmp 0x107
    0x07,
    0x01,

    0xcb, // jmp 0x10a
    0x0a,
    0x01,

    // terminate 
    0x0e, // mvi c, 0x00,
    0x00,
    0xcd, // call 0x0005
    0x05,
    0x00,
  };

  cpu.loadProgram(prog, 0x100);
  cpu.setProgramCounter(0x100);
  cpu.execute();
}

int main(void)
{
  //jnzTest();
  //printTest();
  //stackTest();
  jmpTest();
}