#include <iostream>

#include <boost/format.hpp>

#include "emu.hh"
#include "../argparser/src/argparser.h"


int main(int argc, char *argv[])
{
  struct argparser_t *parser = argparser_new("emu");
  struct option_init_t opt;

  opt.short_name = "-h";
  opt.long_name = "--help";
  opt.help = "show this help message and exit";
  opt.required = false;
  opt.takes_arg = false;
  argparser_from_struct(parser, &opt);

  opt.short_name = "-i";
  opt.long_name = "--interpreter";
  opt.help = "run in interpreter mode";
  opt.required = false;
  opt.takes_arg = false;
  argparser_from_struct(parser, &opt);

  argparser_parse(parser, argc, argv);

  if (argparser_passed(parser, "--help"))
  {
    std::cout << "Intel8080 emulator\n\n";
    argparser_usage(parser);
    return 0;
  }

  argparser_free(parser);

  return 0;
}
