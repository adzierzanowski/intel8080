#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>

#include <boost/algorithm/string.hpp>

#include "emu.hh"


class Interpreter
{
  std::unique_ptr<Emulator> emu;
  void parse_line(std::string line);

  public:
    Interpreter(void);
    void main_loop(void);
};

#endif
