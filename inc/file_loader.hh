#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/dll.hpp>

#include "opcode.hh"


class FileLoader
{
  std::string filename;
  std::vector<uint8_t> binary;
  std::vector<std::string> text;
  std::vector<Opcode> opcodes;

  public:
    static const std::string OPCODES_FILENAME;
    FileLoader(std::string filename);
    void load_hex(void);
    void load_bin(void);
    void load_text(void);
    void load_opcodes(void);
    std::vector<uint8_t> get_binary(void);
    std::vector<std::string> get_text(void);
    std::vector<Opcode> get_opcodes(void);
};

#endif
