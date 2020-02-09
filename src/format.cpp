#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) 
{ 
  int i_hour = seconds / 3600;
  string s_hour;
  if (i_hour <= 9)
  {
    s_hour = "0" + std::to_string(i_hour);
  }
  else 
  {
    s_hour = std::to_string(i_hour);
  }
  int i_minutes = (seconds % 3600) / 60;
  string s_minutes;
  if (i_minutes <= 9)
  {
    s_minutes = "0" + std::to_string(i_minutes);
  }
  else 
  {
    s_minutes = std::to_string(i_minutes);
  }
  int i_seconds = (seconds % 3600) % 60;
  string s_seconds;
  if (i_seconds <= 9)
  {
    s_seconds = "0" + std::to_string(i_seconds);
  }
  else 
  {
    s_seconds = std::to_string(i_seconds);
  }
  return s_hour + ":" + s_minutes + ":" + s_seconds;
}
