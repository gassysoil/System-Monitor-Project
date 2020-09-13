#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  // done: See src/system.cpp
  Processor& Cpu();                   
  
  // done: See src/system.cpp
  std::vector<Process>& Processes();  
  
  // done: See src/system.cpp
  float MemoryUtilization();     
  
  // done: See src/system.cpp
  long UpTime();            
  
  // done: See src/system.cpp
  int TotalProcesses();        
  
  // done: See src/system.cpp
  int RunningProcesses();           
  
  // done: See src/system.cpp
  std::string Kernel();         
  
  // done: See src/system.cpp
  std::string OperatingSystem();      

  // nothing else todo: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif