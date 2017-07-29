#include "Globals.h"

Log::Log()
{
}

Log::~Log()
{
}

void Log::writeLine(Logfile l, std::string data) {
	FILE* lfile;
	switch (l) {
	case LOG_GENERAL:
		lfile = Globals::fs_.openFile(FS_SDMC, Globals::fs_.getDataDirName() +  "/logGeneral.log");
		break;
	case LOG_GRAPHICS:
		lfile = Globals::fs_.openFile(FS_SDMC, Globals::fs_.getDataDirName() + "/logGraphics.log");
		break;
	case LOG_LOGIC:
		lfile = Globals::fs_.openFile(FS_SDMC, Globals::fs_.getDataDirName() + "/logLogic.log");
		break;
	default:
		Error::throwError("Attempted to log an unimplemented log type.");
		//halt exec
	}
	fseek(lfile, 0, SEEK_END);
	fwrite((data).c_str(), sizeof(char), data.size(), lfile);
	fwrite(std::string("\n").c_str(), sizeof(char), 1, lfile);
	fclose(lfile);
	//done
}


Log Log::instance_;

Log& Log::instance() {
	return instance_;
}

