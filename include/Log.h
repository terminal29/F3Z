#pragma once

#include <FileSystem.h>
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <vector>

enum Logfile {
	LOG_GENERAL,
	LOG_GRAPHICS,
	LOG_LOGIC
};

class Log
{
public:
	
	static Log& instance();
	void writeLine(Logfile l, std::string data);

	// Start profiler for a function
	void profileStart(std::string funcName);

	// End profiler for a function
	void profileEnd(std::string funcName);

	// Write profiler data to disk
	void profileOutput();

private:
	static Log instance_;

	std::vector<std::string> funcNames;
	std::vector<std::vector<std::chrono::microseconds>> funcBeginTimes;
	std::vector<std::vector<std::chrono::microseconds>> funcEndTimes;

	Log();
	~Log();
};

