#include <iostream>

#include <boost/format.hpp>

#define private public
#include "emu.hh"


int main(int argc, char *argv[])
{
  Emulator emu;
  emu.load_program({0x3e, 0x01, 0x06, 0x02, 0x0e, 0x03, 0x76});
  emu.execute();
  printf("a = %d\n", emu.cpu->a);
  return 0;
}
