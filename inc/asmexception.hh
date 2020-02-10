#ifndef ASMEXCEPTION_H
#define ASMEXCEPTION_H


struct assembler_exception : public std::runtime_error
{
  explicit assembler_exception(const std::string& what)
    : std::runtime_error(what) {}
};

#endif
