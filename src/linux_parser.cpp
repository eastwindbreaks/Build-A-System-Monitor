#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
}

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

float LinuxParser::MemoryUtilization() 
{ 
  string key, value;
  string MemTotal, MemFree;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      if (linestream >> key >> value)
      {
        if (key == "MemTotal:")
          MemTotal = value;
        if (key == "MemFree:")
        {
          MemFree = value;
          return (std::stof(MemTotal) - std::stof(MemFree)) / std::stof(MemTotal);
        }
      }
    }
  }
  return 0;
}

long LinuxParser::UpTime() 
{ 
  string uptime = "0";
  string item1, item2;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      if (linestream >> item1 >> item2)
      {
        string temp(item1, 0, item1.find_first_of('.'));
        uptime = temp;
        return std::stol(uptime);
      }
    }
  }
  return std::stol(uptime);
}

long LinuxParser::Jiffies() { return 0; }

long LinuxParser::ActiveJiffies() { return 0; }

long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() 
{ 
  string key, value;
  string totalprocesses;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      if (linestream >> key >> value)
      {
        if (key == "processes")
        {
          totalprocesses = value;
          return std::stoi(totalprocesses);
        }
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() 
{ 
  string key, value;
  string runningprocesses;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      if (linestream >> key >> value)
      {
        if (key == "procs_running")
        {
          runningprocesses = value;
          return std::stoi(runningprocesses);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) 
{ 
  string line = "0";
  std::ifstream stream(kProcDirectory+ to_string(pid) + kCmdlineFilename);
  if(stream.is_open())
  {
    while (std::getline(stream, line))
    {
      return line;
    }
  } 
  return line;
}

string LinuxParser::Ram(int pid) 
{ 
  string line = "0";
  string key, value;
  std::ifstream stream(kProcDirectory+ to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      if (linestream >> key >> value)
      {
        if (key == "VmSize:")
          return to_string((int) std::stof(value) / 1000);
      }
    }
  }
  return line;
}

string LinuxParser::Uid(int pid) 
{ 
  string line;
  string key;
  string value = "0";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while ( std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "Uid:")
          return value;
      }
    }
  } 
  return value;
}

string LinuxParser::User(int pid) 
{ 
  string username = "0";
  string letter_x;
  string uid;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> letter_x >> uid)
      {
        if (uid == LinuxParser::Uid(pid))
          return username;
      }
    }
  } 
  return username;
}

long LinuxParser::UpTime(int pid) 
{ 
  string line;
  string item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, item13, s_utime, s_stime, s_cutime, s_cstime, item18, item19, item20, item21, s_starttime;
  float starttime = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (std::getline(stream, line))
  {
     std::istringstream linestream(line);
     while (linestream >> item1 >> item2 >> item3 >> item4 >> item5 >> item6 >> item7 >> item8 >> item9 >> item10 >> item11 >> item12 >> item13 >> s_utime >> s_stime >> s_cutime >> s_cstime >> item18 >> item19 >> item20 >> item21 >> s_starttime)
          {
              starttime = std::stof(s_starttime);
              return LinuxParser::UpTime() - (long) (starttime / sysconf(_SC_CLK_TCK));
          }
  }
  return 0;
}
