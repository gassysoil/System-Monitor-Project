#include "processor.h"
#include "linux_parser.h"

using LinuxParser::CPUTime;
using std::vector;
using std::array;

//udpated

Processor::Processor() { GetLinuxParsesForJiffies(prev_cpu_jiffies_); }

float Processor::Utilization() {
  GetLinuxParsesForJiffies(current_cpu_jiffies_);
  CalculateDeltaTime(current_cpu_jiffies_, prev_cpu_jiffies_);
  const float utilization = CalculateUtilization();
  current_cpu_jiffies_.swap(prev_cpu_jiffies_);
  return utilization;
}

void Processor::CalculateDeltaTime(array<long, 3>& first_cpu_jiffies,
                                   array<long, 3>& second_cpu_jiffies) {
  for (int i = 0; i < 3; ++i) {
    delta_jiffies_[i] = first_cpu_jiffies[i] - second_cpu_jiffies[i];
  }
}

void Processor::GetLinuxParsesForJiffies(array<long, 3>& cpu_jiffies) {
  cpu_jiffies[CPUTime::kIdleTime_] = LinuxParser::IdleJiffies();
  cpu_jiffies[CPUTime::kActiveTime_] = LinuxParser::ActiveJiffies();
  cpu_jiffies[CPUTime::kTotalTime_] = LinuxParser::Jiffies();
}

float Processor::CalculateUtilization() {
  const float small_number_buffer = 0.0000000001;
  const float utilization =
      delta_jiffies_[CPUTime::kActiveTime_] /
      (delta_jiffies_[CPUTime::kTotalTime_] + small_number_buffer);
  return utilization;
}

long Processor::getTotalJiffies() {
  return current_cpu_jiffies_[CPUTime::kTotalTime_];
}

void Processor::UpdatePrevJiffies() {
  for (size_t i = 0; i < prev_cpu_jiffies_.size(); ++i) {
    prev_cpu_jiffies_[i] = current_cpu_jiffies_[i];
  }
}