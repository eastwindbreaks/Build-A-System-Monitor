#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include "process.h"
#include "processor.h"
#include "system.h"
//
#include "linux_parser.h"
#include <iostream>
using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() 
{ 
  return this->cpu_; 
}

vector<Process>& System::Processes() 
{ 
  vector<Process> allProcess;
  vector<int> pids = LinuxParser::Pids();
  int i = pids.size();
  allProcess.resize(i);
  for (int j = 0; j < i; j++)   //set pid for each process
  {
    allProcess[j].setpid(pids[j]);
  }
  std::sort(allProcess.begin(), allProcess.end()); 
  std::reverse(allProcess.begin(), allProcess.end());
  this->processes_ = allProcess;
  return processes_; 
}

std::string System::Kernel() 
{ 
  return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() 
{ 
  return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() 
{ 
  return LinuxParser::OperatingSystem(); 
}

int System::RunningProcesses() 
{ 
  return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() 
{ 
  return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() 
{ 
  return LinuxParser::UpTime(); 
}
