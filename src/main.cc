#include <iostream>

#include <boost/format.hpp>

#include "emu.hh"
#include "interpreter.hh"
#include "file_loader.hh"
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

  opt.short_name = "-v";
  opt.long_name = "--verbose";
  opt.help = "be verbose";
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
  if (argparser_passed(parser, "--interpreter"))
  {
    Interpreter interpreter;
    interpreter.main_loop();
    return 0;
  }
  if (parser->positional_count > 1)
  {
    std::string filename = std::string(parser->positional[1]);
    if (boost::algorithm::ends_with(filename, ".hex"))
    {
      FileLoader hexfile_loader(filename);
      hexfile_loader.load_hex();
      Emulator emu;
      if (argparser_passed(parser, "--verbose"))
        emu.set_verbose_execution(true);
      emu.load_program(hexfile_loader.get_binary());
      emu.execute();
    }
    else
    {
      std::cerr << "Unrecognized file format for " << filename << ".\n";
      argparser_free(parser);
      return 1;
    }
  }

  argparser_free(parser);

  return 0;
}
