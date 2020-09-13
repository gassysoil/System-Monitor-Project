#ifndef PROCESS_H
#define PROCESS_H

#include <string>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // Constructor for Process class
  Process(const int pid, const long total_jiffies);
  
  // done: See src/process.cpp
  int Pid();                       
  
  // done: See src/process.cpp
  std::string User();                   
  
  // done: See src/process.cpp
  std::string Command();                
  
  // done: See src/process.cpp
  float CpuUtilization();        
  
  // done: See src/process.cpp
  std::string Ram();          
  
  // done: See src/process.cpp
  long int UpTime();        
  bool operator<(Process const& a) const;  
  void calculateCpuUtilization(const long total_jiffies);

  
  // done: Declare any necessary private members
 private:
  int pid_;
  float cpu_utilization_;
};

#endif