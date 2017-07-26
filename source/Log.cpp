#include "Log.h"

Log::Log()
{
}

void Log::writeLine(Logfile l, std::string data) {
	std::fstream* lfile;
	switch (l) {
	case LOG_GENERAL:
		lfile = Game::instance().getFS().openFile(FS_SDMC, Game::instance().getFS().getDataDirName() +  "/logGeneral.log");
		break;
	case LOG_GRAPHICS:
		lfile = Game::instance().getFS().openFile(FS_SDMC, Game::instance().getFS().getDataDirName() + "/logGraphics.log");
		break;
	case LOG_LOGIC:
		lfile = Game::instance().getFS().openFile(FS_SDMC, Game::instance().getFS().getDataDirName() + "/logLogic.log");
		break;
	default:
		Error::throwError("Attempted to log an unimplemented log type.");
		//halt exec
	}
	*lfile << data << std::endl;
	delete lfile;
	//done
}


Log::~Log()
{
}
