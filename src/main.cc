#include <iostream>

#include <boost/format.hpp>

#define private public
#include "emu.hh"


int main(int argc, char *argv[])
{
  Emulator emu;
  emu.load_program({
    0x21, 0xde, 0xad, // lxi hl, 0xadde
    0x31, 0x00, 0x01, // lxi sp, 0x100
    0x01, 0xbe, 0xef, // lxi bc, 0xefbe
    0xc5, // push bc
    0xe3, // xthl
    0x76,
    0x76
  });
  emu.execute();
  return 0;
}
