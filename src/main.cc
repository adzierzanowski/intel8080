#include <iostream>

#include <boost/format.hpp>

#include "emu.hh"


int main(int argc, char *argv[])
{
  Emulator emu;
  emu.load_program({0x01, 0x00, 0x00, 0x76}, 0);
  emu.execute();
  return 0;
}
