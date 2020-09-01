#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  float total = 1.0;
  float free = 0.0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    
    std::getline(stream, line);
    std::istringstream firstline(line);
    firstline >> key >> total;
    
    std::getline(stream, line);
    std::istringstream secondline(line);
    secondline >> key >> free;
  }
  return (total-free)/total;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string token;
  long uJiff, nJiff, kJiff, idleJiff, ioJiff, irqJiff, softJiff, stealJiff;
  if(stream.is_open()){
    while(std::getline(stream, line)) {
      std::istringstream iss(line);
      iss >> token;
      if("cpu" == token) {
        iss >> uJiff >> nJiff >> kJiff >> idleJiff >> ioJiff >> irqJiff >> softJiff >> stealJiff;
        return uJiff + nJiff + kJiff + idleJiff + ioJiff + irqJiff + softJiff + stealJiff;
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  string token;
  int counter = 14;
  long utime, stime, cutime, cstime;
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream iss(line);
    while(--counter) {
      iss >> token;
    }
    iss >> utime >> stime >> cutime >> cstime;
  }
  return utime + stime + cutime + cstime; 
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string token;
  long uJiff, nJiff, kJiff, irqJiff, softJiff, stealJiff, dummy;
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream iss(line);
      iss >> token;
      if("cpu" == token) {
        iss >> uJiff >> nJiff >> kJiff >> irqJiff >> softJiff >> stealJiff;
        return uJiff + nJiff + kJiff + irqJiff + softJiff + stealJiff;
      }
    }
  }
  return 0; 
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string token;
  long dummy, idleJiff, ioJiff;
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream iss(line);
      iss >> token;
      if(token == "cpu") {
        iss >> dummy >> dummy >> dummy >> idleJiff >> ioJiff;
        return idleJiff + ioJiff;
      }
    }
  }
  return 0; 
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuValues{};
  string line;
  string key;
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq
             >> softirq >> steal >> guest >> guest_nice) {
        if (key == "cpu") {
          cpuValues.emplace_back(user);
          cpuValues.emplace_back(nice);
          cpuValues.emplace_back(system);
          cpuValues.emplace_back(idle);
          cpuValues.emplace_back(iowait);
          cpuValues.emplace_back(irq);
          cpuValues.emplace_back(softirq);
          cpuValues.emplace_back(steal);
          cpuValues.emplace_back(guest);
          cpuValues.emplace_back(guest_nice);
          return cpuValues;
        }
      }
    }
  }
  return cpuValues; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "process") {
          return value;
        }
      }
    }
  }
  return value; 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
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

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  std::getline(stream, line);
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  string token;
  long memSize;
  while(std::getline(stream, line)){
    std::istringstream iss(line);
    while(iss >> token) {
      if(token == 'vimSize') {
        iss >> memSize;
        return to_string(memSize/1000);
      }
    }
  }
  return ""; 
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  string token;
  string uid;
  while(std::getline(stream, line)){
    std::istringstream iss(line);
    while(iss >> token) {
      if(token == 'Uid') {
        iss >> uid;
        return uid;
      }
    }
  }
  return "";
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  string line;
  string token;
  string user;
  while (std::getline(stream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream iss(line);
    iss >> user;
    iss >> token >> token;
    if(token == uid) {
      return user;
    }
  }
  return "";
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  string token;
  int counter = 22;
  long processStartTime;
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream iss(line);
    while (--counter) {
      iss >> token;
    }
    iss >> processStartTime;
  }
  return LinuxParser::UpTime()-(processStartTime/sysconf(_SC_CLK_TCK));
}