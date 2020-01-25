#include <iostream>

#include <boost/format.hpp>

#define private public
#include "emu.hh"


int main(int argc, char *argv[])
{
  Emulator emu;
  emu.load_program({0x00, 0xc9, 0xc7, 0x76});
  emu.cpu->pc = 2;
  emu.execute();
  return 0;
}
