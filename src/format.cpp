#include <string>

#include "format.h"

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::TimeHelper(int time) {
  if (time < 10) {
    return '0' + to_string(time);
  }
  return to_string(time);
}

string Format::ElapsedTime(long seconds) {
  int hrs = 0;
  int min = 0;
  int sec = 0;
  
  hrs = seconds/3600;
  seconds = seconds%3600;
  min = seconds/60;
  seconds = seconds%60;
  sec = seconds; 
  
  return TimeHelper(hrs) + ':' + TimeHelper(min) + ':' + TimeHelper(sec); 
}