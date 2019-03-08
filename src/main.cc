#ifndef _WIN32
  #include <unistd.h>
#endif

#include "Intel8080.h"
#include "argparser/Argparser.h"

int main(int argc, char *argv[])
{
  Argparser parser("i8080emu", "Intel 8080 emulator");
  parser.addOption("--file").shortName("-f").takesArgument().help("Filename of an executable binary");
  parser.addOption("--help").shortName("-h").help("Show this help message and exit");
  parser.addOption("--quiet").shortName("-q").help("No instructions output");
  parser.addOption("--verbose").shortName("-v").help("Verbose debug output (registers, etc.)");
  parser.parse(argc, argv);

  if (argc > 1)
  {
    if (parser.passed("--help"))
    {
      parser.usage();
      exit(0);
    }

    Intel8080 cpu;
    cpu.setProgramCounter(0x100);

    #ifdef _WIN32
      cpu.formattedOutput = false;
    #else
      if (!isatty(1))
        cpu.formattedOutput = false;
    #endif

    cpu.loadProgram(parser.getValue("--file"), 0x100);
    if (parser.passed("-q"))
      cpu.debugOutput = false;
    else if (parser.passed("-v"))
      cpu.verboseDebugOutput = true;
    cpu.execute();
  }
  else
  {
    parser.usage();
  }

  return 0;
}
