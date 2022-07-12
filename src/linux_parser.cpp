#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

const vector<unsigned int> idxIdle {3,4};

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  float memTotal = std::numeric_limits<float>::min(), memFree = std::numeric_limits<float>::min();
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      string index, value;
      std::istringstream linestream(line);
      linestream >> index >> value;
      if (index == "MemTotal:")
      {
        memTotal = stof(value);
      }
      else if (index == "MemFree:")
      {
        memFree = stof(value);
      }
      if (memTotal != std::numeric_limits<float>::min() && memFree != std::numeric_limits<float>::min())
      {
        return (memTotal - memFree) / memTotal;
      }
    }
  }
  return 0.;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime()
{
  string upTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
    return stol(upTime);
  }
  return 0; 
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  long jiffies;
  for(auto element : CpuUtilization())
  {
    jiffies+=stol(element);
  }
  return jiffies;
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  long jiffies = 0;
  string line, element;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i=1;i<=17;i++){
      linestream>>element;
      if(i>=14 && i<=17)
        jiffies+=stol(element);
    }
  }
  return jiffies;
}
// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  long jiffies=0;
  unsigned int idx=0;
  for(auto element : CpuUtilization())
  {
    if(std::find(idxIdle.begin(), idxIdle.end(), idx) == idxIdle.end())
      jiffies+=stol(element);
    idx++;
  }
  return jiffies;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  long jiffies=0;
  unsigned int idx=0;
  for(auto element : CpuUtilization())
  {
    if(std::find(idxIdle.begin(), idxIdle.end(), idx) != idxIdle.end())
      jiffies+=stol(element);
    idx++;
  }
  return jiffies;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  vector<string> cpuUtil;
  string line, element;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> element;
    if(element == "cpu"){
      while(linestream){
        linestream >> element;
        cpuUtil.push_back(element);
      }
      return cpuUtil;
    }
  }
  return vector<string>{};
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  int totProc = std::numeric_limits<int>::max();
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line))
    {
      string index, value;
      std::istringstream linestream(line);
      linestream >> index >> value;
      if (index == "processes")
      {
        totProc = stoi(value);
      }
      if (totProc != std::numeric_limits<int>::max())
      {
        return totProc;
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  int runProc = std::numeric_limits<int>::max();
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line))
    {
      string index, value;
      std::istringstream linestream(line);
      linestream >> index >> value;
      if (index == "procs_running")
      {
        runProc = stoi(value);
      }
      if (runProc != std::numeric_limits<int>::max())
      {
        return runProc;
      }
    }
  }
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{ 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    return line;
  }
  return string();
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line))
    {
      string index, value;
      std::istringstream linestream(line);
      linestream >> index >> value;
      if (index == "VmSize:")
      {
        return  std::to_string(stoi(value) / 1000);
      }
    }
  }
  return string();
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]])
{
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line))
    {
      string index, value;
      std::istringstream linestream(line);
      linestream >> index >> value;
      if (index == "Uid:")
      {
        return value;
      }
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  string uid = LinuxParser::Uid(pid);
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      string index, value;
      std::istringstream linestream(line);
      linestream >> value >> index >> index;
      if (index == uid)
      {
        return value;
      }
    }
  }
  return string();
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
  string line,element;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i=0;i<22;i++){
      linestream >> element;
    }
    return LinuxParser::UpTime() - stol(element) / sysconf(_SC_CLK_TCK);
  }
  return 0;
}
