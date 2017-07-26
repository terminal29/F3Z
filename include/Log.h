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
	Log();
	~Log();
	
	
	void writeLine(Logfile l, std::string data);

private:
	std::string logFolder_ = "/logs";
};

