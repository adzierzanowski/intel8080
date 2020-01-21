#include <iostream>

#include <boost/format.hpp>

#define private public
#include "emu.hh"


int main(int argc, char *argv[])
{
  Emulator emu;
  emu.load_program({
    0x3e, 156, // mvi a, 42
    0x21, 0x00, 0x01, // lxi h, 0x100
    0x36, 100, // mvi m, 100
    0x86, // add a, m
    0x76 // hlt
  });
  emu.execute();
  printf("a = %d\n", emu.cpu->a);
  return 0;
}
