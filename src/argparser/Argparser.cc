#include "Argparser.h"

Argparser::Argparser(std::string name, std::string description) : name(name), description(description)
{
}

OptionAdder Argparser::addOption(std::string longName)
{
  return OptionAdder(longName, this);
}

void Argparser::parse(int argc, char *argv[])
{
  for (int i = 1; i < argc; i++)
  {
    std::string opt(argv[i]);
    auto iOpt = findOption(opt);
    if (iOpt.first > -1)
    {
      options[iOpt.first].passed = true;
      if (iOpt.second.takesArgument)
      {
        if (i == argc - 1)
        {
          std::cerr << "Error: option " << iOpt.second.longName << " requires an argument.\n";
          exit(1);
        }

        std::string arg(argv[i+1]);
        if (findOption(arg).first > -1)
        {
          std::cerr << "Error: option " << iOpt.second.longName << " requires an argument.\n";
          exit(1);
        }
        options[iOpt.first].value = arg;
      }
    }
    else
    {
      positional.push_back(opt);
    }
  }
}

std::pair<int, Option> Argparser::findOption(std::string name)
{
  for (size_t i = 0; i < options.size(); i++)
  {
    if (options[i].shortName == name || options[i].longName == name)
      return std::make_pair(i, options[i]);
  }
  return std::make_pair(-1, Option(""));
}

void Argparser::usage(void)
{
  std::cout << "usage: " << name << " ";
  for (Option opt : options)
  {
    if (opt.required)
      std::cout << opt.longName << " ";
    else
    {
      std::cout << "[" << opt.longName << "] ";
    }
  }
  std::cout << "\n\n";
  std::cout << description << "\n\n";

  for (Option opt : options)
  {
    std::cout << "  ";
    if (opt.shortName != "")
      std::cout << opt.shortName << ", ";
    std::cout << opt.longName << "\t" << opt.help;
    std::cout << "\n";
  }
}

std::string Argparser::getValue(std::string name)
{
  auto iOpt = findOption(name);
  if (iOpt.first > -1)
    return iOpt.second.value;
  return "";
}

bool Argparser::passed(std::string name)
{
  auto iOpt = findOption(name);
  if (iOpt.first > -1)
    return iOpt.second.passed;
  return false;
}
