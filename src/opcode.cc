#include "opcode.hh"


Opcode::Opcode(
  uint8_t number,
  uint8_t length,
  uint8_t cycles,
  std::string mnemonic,
  std::string description
) :
  number{number},
  length{length},
  cycles{cycles},
  mnemonic{mnemonic},
  description{description}
{
}

std::vector<Opcode> load_opcodes(std::string filename)
{
  std::vector<Opcode> opcodes;
  std::ifstream opfile(filename);
  if (opfile.fail())
  {
    std::cerr << "Failed to load opcodes: make sure '" << filename << "' exists.\n";
    std::exit(1);
  }

  std::string line;
  std::getline(opfile, line); // Skip the header
  int i = 0;
  while (std::getline(opfile, line))
  {
    if (line != "")
    {
      std::vector<std::string> split_line;
      boost::split(split_line, line, boost::is_any_of(","));

      uint8_t op_number = i;
      std::string op_mnemonic = split_line[1];
      uint8_t op_length = std::stoul(split_line[2]);
      uint8_t op_cycles = std::stoul(split_line[3]);
      std::string op_description = split_line[4];

      boost::trim(op_mnemonic);
      boost::trim(op_description);

      Opcode op(op_number, op_length, op_cycles, op_mnemonic, op_description);
      opcodes.push_back(op);

      i++;
    }
  }

  return opcodes;
}
