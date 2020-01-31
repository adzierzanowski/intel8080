#include "interpreter.hh"


Interpreter::Interpreter() : emu(std::make_unique<Emulator>()) {}

void Interpreter::parse_line(std::string line)
{
  std::vector<std::string> splitline;
  boost::split(splitline, line, boost::is_any_of(" "));

  if (splitline[0] == ".dump")
  {
    std::cout << emu->dump() << "\n";
  }
}

void Interpreter::main_loop(void)
{
  std::string input;
  while (true)
  {
    std::cout << "8080> ";
    //std::cin >> input;
    std::getline(std::cin, input);
    parse_line(input);
    //std::cout << "OUT: " << input << std::endl;
  }
}
