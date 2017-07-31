#include "Log.h"

Log::Log()
{
}

Log::~Log()
{
}

void Log::profileStart(std::string funcName) {
	for (int i = 0; i < funcNames.size(); i++) {
		if (funcNames.at(i) == funcName) {
			funcBeginTimes.at(i).push_back(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()));
			return;
		}
	}
	funcNames.push_back(funcName);
	std::vector<std::chrono::microseconds> v;
	std::vector<std::chrono::microseconds> u;
	funcBeginTimes.push_back(v);
	funcEndTimes.push_back(u);
	funcBeginTimes.at(funcNames.size() - 1).push_back(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()));
}

void Log::profileEnd(std::string funcName) {
	for (int i = 0; i < funcNames.size(); i++) {
		if (funcNames.at(i) == funcName) {
			funcEndTimes.at(i).push_back(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()));
			return;
		}
	}
	Error::throwError("Profiler ended for " + funcName + " but was never started.");
}

void Log::profileOutput() {
	std::stringstream info;
	info << std::endl << "Name\t\t\t\tCall Count\t\t\t\tAverage u" << std::endl;
	for (int i = 0; i < funcNames.size(); i++) {
		std::vector<std::chrono::microseconds> begins = funcBeginTimes.at(i);
		std::vector<std::chrono::microseconds> ends = funcEndTimes.at(i);
		/*
		if (begins.size() != ends.size()) {
			Error::throwError("Profiler data of " + funcNames.at(i) + " is not of matching lengths");
			//exec stop
		}*/
		int callCount = begins.size();
		long long total = 0;
		for (int i = 0; i < begins.size() && i < ends.size(); i++) {
			total += (ends.at(i) - begins.at(i)).count();
		}
		long double avg = total/(long double)callCount; // dunno if this much precision will be needed but it doesnt hurt

		info << funcNames.at(i) << "\t\t\t\t" << callCount << "\t\t\t\t" << avg << std::endl;
	}
	
	Globals::log_.writeLine(Logfile::LOG_GENERAL, info.str());

}

void Log::writeLine(Logfile l, std::string data) {
	FILE* lfile;
	switch (l) {
	case LOG_GENERAL:
		lfile = Globals::fs_.openFile(FS_SDMC, Globals::fs_.getDataDirName() + "/logGeneral.log");
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

