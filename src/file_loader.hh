#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class FileLoader
{
  std::string filename;
  std::vector<uint8_t> content;

  public:
    FileLoader(std::string filename);
    void load_hex(void);
    void load_bin(void);
};

#endif
