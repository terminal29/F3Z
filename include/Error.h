#pragma once
#include <string>

#include <FileSystem.h>
#include <Log.h>
#include <3ds.h>

class Error
{
public:
	static void throwError(std::string errorText);

private:
	Error();
	~Error();
	static void haltExec();
};

