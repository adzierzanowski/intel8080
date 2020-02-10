#include "asmconstraint.hh"


std::string to_string(const Constraint& constraint)
{
  switch (constraint)
  {
    case Constraint::ABCDEHLM:
      return "A, B, C, D, E, H, L or M";
      break;
    
    case Constraint::BD:
      return "B or D";
      break;
    
    case Constraint::BDHPSW:
      return "B, D, H or PSW";
      break;
    
    case Constraint::BDHSP:
      return "B, D, H or SP";
      break;
    
    case Constraint::IMM16:
      return "16-bit immediate";
      break;
    
    case Constraint::IMM8:
      return "8-bit immediate";
      break;

    case Constraint::NONE:
      return "any";
      break;
  
    default:
      return "<UNKNOWN>";
      break;
  }
}
