#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <array>
#include <vector>

using std::vector;
using std::array;

// this class has been updated
class Processor {
 public:
  // done: See src/processor.cpp
  float Utilization();  
  long getTotalJiffies();
  Processor();

  // done: Declare any necessary private members
 private:
  void CalculateDeltaTime(array<long, 3>&, array<long, 3>&);
  virtual void GetLinuxParsesForJiffies(array<long, 3>&);
  float CalculateUtilization();
  void UpdatePrevJiffies();

  array<long, 3> current_cpu_jiffies_;
  array<long, 3> prev_cpu_jiffies_;
  array<float, 3> delta_jiffies_;
};

#endif