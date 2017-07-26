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
	
	//template <typename T>
	//void writeLine(Logfile l, T data);

private:
	std::string logFolder_ = "/F3Z_Data";
};

