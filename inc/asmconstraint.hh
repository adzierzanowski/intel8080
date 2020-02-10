#ifndef ASMCONSTRAINT_H
#define ASMCONSTRAINT_H

#include <cstdint>
#include <string>

#include "cpu.hh"


enum class Constraint : uint32_t
{
  NONE,
  IMM8,
  IMM16,
  BD,
  BDHSP,
  BDHPSW,
  ABCDEHLM,
};

std::string to_string(const Constraint& constraint);

#endif
