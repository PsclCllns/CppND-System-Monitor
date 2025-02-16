#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtil; }

// DONE: Return the command that generated this process
string Process::Command() { return command; }

// DONE: Return this process's memory utilization
string Process::Ram() { return ram; }

// DONE: Return the user (name) that generated this process
string Process::User() { return user; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime; }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return this->cpuUtil < a.cpuUtil; }

bool Process::operator>(Process const& a) const { return this->cpuUtil > a.cpuUtil; }