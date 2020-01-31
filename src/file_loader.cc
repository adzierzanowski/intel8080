#include "file_loader.hh"


FileLoader::FileLoader(std::string filename) : filename{filename} {}

void FileLoader::load_hex(void)
{
  std::ifstream hexfile(filename);
  std::string line;

  int line_number = 0;
  while (std::getline(hexfile, line))
  {
    line_number++;
    if (line[0] != ':')
    {
      std::cerr << filename << ":" << line_number << ": ";
      std::cerr << "Bad hex file: no ':' at the beginning of the line.\n";
      return;
    }

    std::string bytes_in_line_str, addr_str;
    try
    {
      bytes_in_line_str = line.substr(1, 2);
      addr_str = line.substr(2, 4);
    }
    catch (const std::out_of_range& _)
    {
      std::cerr << filename << ":" << line_number << ": ";
      std::cerr << "Bad hex file: no meta.\n";
      return;
    }

    uint8_t bytes_in_line = (uint8_t) std::stoi(bytes_in_line_str, 0, 0x10);
    uint16_t addr = (uint16_t) std::stoi(addr_str, 0, 0x10);
    (void) addr; // Silence unused warning.

    std::string record_type;
    try
    {
      record_type = line.substr(7, 2);
    }
    catch(const std::out_of_range& _)
    {
      std::cerr << filename << ":" << line_number << ": ";
      std::cerr << "Bad hex file: no record type.\n";
      return;
    }

    if (record_type == "00")
    {
      for (int i = 0; i < bytes_in_line; i++)
      {
        std::string byte_str;

        try
        {
          byte_str = line.substr(9+i*2, 2);
          binary.push_back((uint8_t) std::stoi(byte_str, 0, 0x10));
        }
        catch (const std::out_of_range& _)
        {
          std::cerr << filename << ":" << line_number << ":" << i << ": ";
          std::cerr << "Bad hex file: incomplete data.\n";
          return;
        }
        catch (const std::invalid_argument& _)
        {
          std::cerr << filename << ":" << line_number << ":" << i << ": ";
          std::cerr << "Bad hex file: invalid byte (" << byte_str << ").\n";
          return;
        }
      }
    }
    else if (record_type == "01")
    {
      return;
    }
    else
    {
      std::cerr << filename << ":" << line_number << ": ";
      std::cerr << "Bad hex file: unsupported record type (" << record_type << ").\n";
      return;
    }
  }
}

void FileLoader::load_bin(void)
{

}

void FileLoader::load_text(void)
{
  std::string line;
  std::ifstream text_file(filename);

  while (std::getline(text_file, line))
  {
    text.push_back(line);
  }
}

std::vector<uint8_t> FileLoader::get_binary(void)
{
  return binary;
}

std::vector<std::string> FileLoader::get_text(void)
{
  return text;
}
