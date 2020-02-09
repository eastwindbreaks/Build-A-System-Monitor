#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() 
{ 
  std::string cpu;
  std::string line;
  std::string cpu_i;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    std::vector<float> each_time;
    for (int i = 0; i <8; i++)
    {
      linestream >> cpu_i;
      each_time.push_back(stof(cpu_i));
    }
    float sum_cputime = 0;
    for (auto n : each_time)
    {
      sum_cputime = sum_cputime + n;
    }
    float percentage = (sum_cputime - each_time[3] - each_time[4]) / sum_cputime;
    return percentage;
  }
  return 0;
}
