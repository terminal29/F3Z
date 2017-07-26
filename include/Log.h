#pragma once
#include "FileSystem.h"
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
	
	template <typename T>
	void writeLine(Logfile l, T data);

};

