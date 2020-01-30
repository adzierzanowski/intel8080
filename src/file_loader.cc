#include "file_loader.hh"


FileLoader::FileLoader(std::string filename) : filename{filename} {}

void FileLoader::load_hex(void)
{
  std::ifstream hexfile(filename);
  std::string line;

  while (std::getline(hexfile, line))
  {
    std::cout << line << std::endl;
  }

}

void FileLoader::load_bin(void)
{

}
