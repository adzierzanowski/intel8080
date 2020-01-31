#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class FileLoader
{
  std::string filename;
  std::vector<uint8_t> binary;
  std::vector<std::string> text;

  public:
    FileLoader(std::string filename);
    void load_hex(void);
    void load_bin(void);
    void load_text(void);
    std::vector<uint8_t> get_binary(void);
    std::vector<std::string> get_text(void);
};

#endif
