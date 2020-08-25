#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;

Processor::Processor() : prevTotalCpuTime(0.0), prevIdleCpuTime(0.0){};

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
  vector<long> cpuValues = stolVector(LinuxParser::CpuUtilization());
  float currTotalCpuTime = cpuValues[LinuxParser::kUser_] + cpuValues[LinuxParser::kNice_] +
    cpuValues[LinuxParser::kSystem_] + cpuValues[LinuxParser::kIdle_] +
    cpuValues[LinuxParser::kIOwait_] + cpuValues[LinuxParser::kIRQ_] +
    cpuValues[LinuxParser::kSoftIRQ_] + cpuValues[LinuxParser::kSteal_];
  float currIdleCpuTime = cpuValues[LinuxParser::kIdle_] + cpuValues[LinuxParser::kIOwait_];
  float diffIdle = currIdleCpuTime - prevIdleCpuTime;
  float diffTotal = currTotalCpuTime - prevTotalCpuTime;
  float diffUsage = (diffTotal - diffIdle) / diffTotal;
  prevIdleCpuTime = currIdleCpuTime;
  prevTotalCpuTime = currTotalCpuTime;
  return diffUsage; 
}

vector<long> Processor::stolVector(vector<string> values) {
  vector<long> converted{};
  for (int i = 0; i < (int)values.size(); i++) {
    try {
      converted.emplace_back(std::stol(values[i]));
    } catch (const std::invalid_argument& arg) {
      converted.emplace_back((long)0);
    }
  }
  return converted;
}