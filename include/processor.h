#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp
  Processor();
  // DONE: Declare any necessary private members
 private:
  float prevTotalCpuTime;
  float prevIdleCpuTime;
  vector<long> stolVector(vector<string> values);
};

#endif