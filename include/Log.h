#pragma once

#include <FileSystem.h>
#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <vector>
#include <unordered_map>

enum Logfile
{
	LOG_GENERAL,
	LOG_GRAPHICS,
	LOG_LOGIC
};

class Log
{
public:
	static Log &instance();
	void writeLine(Logfile l, std::string data);

	// Start profiler for a function
	void profileStart(std::string funcName);

	// End profiler for a function
	void profileEnd(std::string funcName);

	// Write profiler data to disk
	void profileOutput();

	using time_t = std::chrono::microseconds;

private:
	static Log instance_;

	std::unordered_map<std::string, std::vector<std::pair<time_t, time_t>>> profileData_;

	Log();
	~Log();
};
