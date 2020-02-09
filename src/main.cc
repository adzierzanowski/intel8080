#include <algorithm>
#include <iostream>

#include <boost/format.hpp>

#include "assembler.hh"
#include "emu.hh"
#include "interpreter.hh"
#include "file_loader.hh"
#include "../argparser/src/argparser.h"

template <typename T>
bool contains(std::vector<T> haystack, T needle)
{
  return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
}

int main(int argc, char *argv[])
{
  struct argparser_t *parser = argparser_new("i8080");
  struct option_init_t opt;

  opt.short_name = "-b";
  opt.long_name = "--bdos";
  opt.help = "use BDOS syscalls";
  opt.required = false;
  opt.takes_arg = false;
  argparser_from_struct(parser, &opt);

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

  opt.short_name = "-o";
  opt.long_name = "--output";
  opt.help = "output path";
  opt.required = false;
  opt.takes_arg = true;
  argparser_from_struct(parser, &opt);

  opt.short_name = "-v";
  opt.long_name = "--verbose";
  opt.help = "be verbose";
  opt.required = false;
  opt.takes_arg = false;
  argparser_from_struct(parser, &opt);

  argparser_parse(parser, argc, argv);

  if (argparser_passed(parser, "--help") || argc == 1)
  {
    std::cout << "Intel8080 emulator\n\n";
    std::cout << "usage: " << parser->prog_name << " ";
    std::cout << "[-abhiov] [filename]" << "\n\n";
    for ( int i = 0; i < parser->count; i++)
    {
      struct option_t *opt = parser->options[i];
      std::cout << boost::format("  %2s, %-20s %-40s")
        % opt->short_name
        % opt->long_name
        % opt->help << "\n";
    }
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
    FileLoader file(filename);
    std::vector<std::string> fname_split;
    boost::split(fname_split, filename, boost::is_any_of("."));
    std::string extension = fname_split.back();
    std::vector<std::string> exe_exts = {"bin", "hex"};
    std::vector<std::string> asm_exts = {"s", "asm"};

    if (extension == "hex")
    {
      file.load_hex();
    }
    else if (extension == "bin")
    {
      file.load_bin();
    }
    else if (contains(asm_exts, extension))
    {
      file.load_text();
    }

    if (contains(exe_exts, extension))
    {
      Emulator emu;
      if (argparser_passed(parser, "--verbose"))
      {
        emu.set_verbose_execution(true);
      }
      emu.load_program(file.get_binary());
      emu.execute();
    }
    else if (contains(asm_exts, extension))
    {
      if (argparser_passed(parser, "--output"))
      {
        std::vector<uint8_t> binary;
        try
        {
          binary = Assembler::assemble(file.get_text());
        }
        catch (assembler_exception& e)
        {
          std::cerr << "error: " << filename << ":";
          std::cerr << e.what() << std::endl;
          argparser_free(parser);
          return 1;
        }

        std::string output_fname = std::string(argparser_get(parser, "--output"));
        std::ofstream output_file(output_fname, std::ios::out | std::ofstream::binary);
        output_file.write((char *) &binary[0], binary.size());
      }
      else
      {
        std::cerr << "Assembler expects '--output' option.\n";
        argparser_free(parser);
        return 1;
      }
    }
    else
    {
      std::cerr << "Unrecognized file format: '" << filename << "'.\n";
      argparser_free(parser);
      return 1;
    }
  }

  argparser_free(parser);

  return 0;
}
