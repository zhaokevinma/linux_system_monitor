#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return this->pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const{ 
  return (LinuxParser::ActiveJiffies(this->pid_)/sysconf(_SC_CLK_TCK))
    /(float)LinuxParser::UpTime(this->pid_); }

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(this->pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(this->pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(this->pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  if(this->CpuUtilization()<a.CpuUtilization())
    return true;
  else
    return false;
}