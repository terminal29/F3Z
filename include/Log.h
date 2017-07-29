#pragma once

#include "FileSystem.h"
#include "Game.h"
#include <string>
#include <iostream>

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

private:
	static Log instance_;

	Log();
	~Log();
	std::string logFolder_ = "/logs";
};

