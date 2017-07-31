#pragma once
#include "FileSystem.h"
#include "Log.h"
#include <3ds.h>
#include <string>

class Error
{
public:
	static void throwError(std::string errorText);

private:
	Error();
	~Error();
	static void haltExec();
};

