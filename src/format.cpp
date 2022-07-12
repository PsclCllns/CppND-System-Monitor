#include <string>

#include "format.h"

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(const long seconds)
{
  const int hours = seconds / 3600;
  const int min = seconds / 60 - hours * 60;
  const int sec = seconds - hours * 3600 - min * 60;
  
  string formattedTime;
  if(hours<10)
    formattedTime+="0"+to_string(hours)+":";
  else
    formattedTime+=to_string(hours)+":";
  if(min<10)
    formattedTime+="0"+to_string(min)+":";
  else
    formattedTime+=to_string(min)+":";
  if(sec<10)
    formattedTime+="0"+to_string(sec);
  else
    formattedTime+=to_string(sec);
  
  return formattedTime; 
}