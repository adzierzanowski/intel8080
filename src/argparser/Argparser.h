#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <vector>
#include <string>
#include <iostream>
#include "Option.h"

struct Option;
struct OptionAdder;

class Argparser
{
  public:
    Argparser(std::string name, std::string description);

    std::vector<Option> options;
    std::vector<std::string> positional;
    std::string name;
    std::string description;

    OptionAdder addOption(std::string longName);
    void parse(int argc, char *argv[]);
    void usage(void);
    std::pair<int, Option> findOption(std::string name);
    std::string getValue(std::string name);
    bool passed(std::string name);
};

#endif
