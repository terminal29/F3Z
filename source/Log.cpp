#include "Log.h"

Log::Log()
{
}
/*
template <typename T>
void Log::writeLine(Logfile l, T data) {
	std::ifstream* lfile;
	switch (l) {
	case LOG_GENERAL:
		lfile = Game::instance().getFS().openFile(FS_SDMC, Game::instance().getFS().getDataDirPath() + logFolder_ + "/logGeneral.log");
		break;
	case LOG_GRAPHICS:
		lfile = Game::instance().getFS().openFile(FS_SDMC, Game::instance().getFS().getDataDirPath() + logFolder_ + "/logGraphics.log");
		break;
	case LOG_LOGIC:
		lfile = Game::instance().getFS().openFile(FS_SDMC, Game::instance().getFS().getDataDirPath() + logFolder_ + "/logLogic.log");
		break;
	default:
		Error::throwError("Attempted to log an unimplemented log type.");
		//halt exec
	}
	*lfile << data << std::endl;
	delete lfile;
	//done
}
*/

Log::~Log()
{
}
