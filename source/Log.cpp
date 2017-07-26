#include "Log.h"

Log::Log()
{
}

template <typename T>
void Log::writeLine(Logfile l, T data) {

	// Open filestream
	std::ifstream file(filepath);
	if (!file) {
		Error::throwError("Invalid filepath " + )
	}
}


Log::~Log()
{
}
