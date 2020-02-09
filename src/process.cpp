#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
//
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::setpid(int pid_process)
{
  this->pid = pid_process;
}

int Process::Pid() 
{ 
  return this->pid; 
}

float Process::CpuUtilization() 
{ 
  string line;
  string item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, item13, s_utime, s_stime, s_cutime, s_cstime, item18, item19, item20, item21, s_starttime;
  float percentage = 0;
  float uptime = 0, starttime = 0, utime = 0, stime = 0, cutime = 0, cstime = 0;
  uptime = LinuxParser::UpTime();
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(Pid()) + LinuxParser::kStatFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      if(linestream >> item1)
      {
        if (item1 == to_string(Pid()))
        {
          if(linestream >> item2 >> item3 >> item4 >> item5 >> item6 >> item7 >> item8 >> item9 >> item10 >> item11 >> item12 >> item13 >> s_utime >> s_stime >> s_cutime >> s_cstime >> item18 >> item19 >> item20 >> item21 >> s_starttime)
          {
          utime = std::stof(s_utime);
          stime = std::stof(s_stime);
          cutime = std::stof(s_cutime);
          cstime = std::stof(s_cstime);
          starttime = std::stof(s_starttime);
          }
        }
       
      }
    }
  
  }
   percentage = (utime + stime + cutime + cstime) / sysconf(_SC_CLK_TCK) / (uptime - starttime / sysconf(_SC_CLK_TCK));
  return percentage;
}

string Process::Command() 
{ 
  return LinuxParser::Command(Pid());
}

string Process::Ram() 
{ 
  return LinuxParser::Ram(Pid());
}

string Process::User() 
{ 
  return LinuxParser::User(Pid()); 
}

long int Process::UpTime() 
{ 
  return LinuxParser::UpTime(Pid()); 
}

bool Process::operator<(Process & a)
{ 
  if (this->CpuUtilization() < a.CpuUtilization())
      return true;
  else 
    return false;
}
