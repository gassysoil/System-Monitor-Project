#include <dirent.h>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream filestream(kProcDirectory + kVersionFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, unit, line;
  float value;
  vector<float> memory_data{};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    for (int i = 0; i < 5;
         ++i) {  // Only interested in first 5 data points from /proc/meminfo
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
      memory_data.push_back(value);
    }
  }
  float utilization = (memory_data.at(MemoryType::kMemTotal_) -
                       memory_data.at(MemoryType::kMemFree_) -
                       memory_data.at(MemoryType::kMemBuffer_) -
                       memory_data.at(MemoryType::kMemCached_)) /
                      memory_data.at(MemoryType::kMemTotal_);

  return utilization;
}

// revised: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long value{0};
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    return value;
  }
  return value;
}

// done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line, cpu_string;
  long value, total_jiffies{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu_string;  // pop first string
    while (linestream >> value) {
      total_jiffies += value;
    }
  }
  return total_jiffies;
}

// done: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string key, line;
  long int value{0}, active_jiffies{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 13; ++i) {
      linestream >> key;
    }
    for (int i = 0; i < 4; ++i) {
      linestream >> value;
      active_jiffies += value;
    }
    return active_jiffies;
  }
  return active_jiffies;
}

// done: Read and return the number of idle jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line, cpu_string;
  long value, active_jiffies{0};
  vector<int> cpu_data{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu_string; 
    while (linestream >> value) {
      cpu_data.push_back(value);
    }
  }
  active_jiffies =
      cpu_data.at(CPUStates::kUser_) + cpu_data.at(CPUStates::kNice_) +
      cpu_data.at(CPUStates::kSystem_) + cpu_data.at(CPUStates::kIRQ_) +
      cpu_data.at(CPUStates::kSoftIRQ_) + cpu_data.at(CPUStates::kSteal_);
  return active_jiffies;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line, cpu_string;
  long value, idle_jiffies{0};
  vector<int> cpu_data{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu_string;  
    while (linestream >> value) {
      cpu_data.push_back(value);
    }
  }
  idle_jiffies =
      cpu_data.at(CPUStates::kIdle_) + cpu_data.at(CPUStates::kIOwait_);
  return idle_jiffies;
}


//done: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { 
//   string line, info;
//   vector<string> infos;
//   std::ifstream filestream(kProcDirectory + kStatFilename);
//   // Get first line
//   if (filestream.is_open()) {
//     std::getline(filestream, line);
//     std::istringstream linestream(line);
//     // Tokenize
//     while (std::getline(linestream, info, ' ')) {
//       if ((info == "cpu") || (info == "")){
//         continue;
//       }
//       infos.push_back(info);
//     }
//   }
//   return infos;
// }

// done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key, line;
  float value{0.0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value;
}

// done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key, line;
  int value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value;
}

// done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return line;
}

// done: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
float LinuxParser::Ram(int pid) {
  const float kb_to_mb = 1000;
  string key, line;
  float value{0.0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return value / kb_to_mb;
        }
      }
    }
  }
  return value;
}


// done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string key, line, value{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

//done: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, key, value{};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return value;
}

// done: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function

// long LinuxParser::UpTime(int pid) {
//   string key, line;
//   long int value{0};
//   std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
//   if (filestream.is_open()) {
//     std::getline(filestream, line);
//     std::istringstream linestream(line);
//     for (int i = 0; i < 21; ++i) {
//       linestream >> key;
//     }
//     linestream >> value;
//     value = value / sysconf(_SC_CLK_TCK);
//     return value;
//   }
//   return value;
// }


// //revised as suggested
long LinuxParser::UpTime(int pid) {
  string line;
  string var;
  int UpTimePid=0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for(int i = 1; i < 23; i++) {
        linestream >> var;
      }
      int UpTimePid = UpTime() - stol(var) / sysconf(_SC_CLK_TCK);
      return UpTimePid;
    }
  }
return UpTimePid;
}
