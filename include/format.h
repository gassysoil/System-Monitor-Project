#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
  
// done: See src/format.cpp
std::string ElapsedTime(long times);  
std::string AppendZero(const long number);
};  // namespace Format

#endif