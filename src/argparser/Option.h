#ifndef ARGPARSER_OPTION_H
#define ARGPARSER_OPTION_H

#include <string>
#include "Argparser.h"

class Argparser;

struct Option {
    std::string shortName = "";
    std::string longName;
    std::string help = "";
    std::string value = "";
    bool takesArgument = false;
    bool required = false;
    bool passed = false;

    Option(std::string longName);
};

struct OptionAdder {
    Argparser *parser;
    int index;

    OptionAdder(std::string longName, Argparser *parser);
    OptionAdder takesArgument(void);
    OptionAdder shortName(std::string shortName);
    OptionAdder required(void);
    OptionAdder help(std::string message);
};

#endif
