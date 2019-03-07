#include "Option.h"

Option::Option(std::string longName) : longName(longName)
{
}

OptionAdder::OptionAdder(std::string longName, Argparser *parser) : parser(parser)
{
  parser->options.push_back(Option(longName));
  index = parser->options.size() - 1;
}

OptionAdder OptionAdder::takesArgument(void)
{
  parser->options[index].takesArgument = true;
  return *this;
}

OptionAdder OptionAdder::help(std::string message)
{
  parser->options[index].help = message;
  return *this;
}

OptionAdder OptionAdder::required(void)
{
  parser->options[index].required = true;
  return *this;
}

OptionAdder OptionAdder::shortName(std::string shortName)
{
  parser->options[index].shortName = shortName;
  return *this;
}