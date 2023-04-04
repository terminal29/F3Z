#include "Log.h"

Log::Log()
{
}

Log::~Log()
{
}

void Log::profileStart(std::string funcName)
{
	const auto now = std::chrono::duration_cast<time_t>(std::chrono::system_clock::now().time_since_epoch());
	if (profileData_.find(funcName) == profileData_.end())
	{
		profileData_[funcName].emplace_back(now, time_t{0});
	}
	else
	{
		profileData_[funcName] = std::vector{std::pair<time_t, time_t>(now, time_t{0})};
	}
}

void Log::profileEnd(std::string funcName)
{
	const auto now = std::chrono::duration_cast<time_t>(std::chrono::system_clock::now().time_since_epoch());
	if (profileData_.find(funcName) != profileData_.end())
	{
		profileData_[funcName].back().second = now;
	}
	else
	{
		Error::throwError("Profiler ended for " + funcName + " but was never started.");
	}
}

void Log::profileOutput()
{
	std::stringstream info;
	info << std::endl
		 << "Name\t\t\t\tCall Count\t\t\t\tAverage u" << std::endl;
	for (auto &func : profileData_)
	{
		info << func.first << "\t\t\t\t" << func.second.size() << "\t\t\t\t";
		time_t total{0};
		for (auto &call : func.second)
		{
			total += call.second - call.first;
		}
		info << total.count() / func.second.size() << std::endl;
	}

	Log::instance().writeLine(Logfile::LOG_GENERAL, info.str());
}

void Log::writeLine(Logfile l, std::string data)
{
	FILE *lfile;
	switch (l)
	{
	case LOG_GENERAL:
		lfile = FileSystem::instance().openFile(FS_SDMC, FileSystem::instance().getDataDirName() + "/logGeneral.log");
		break;
	case LOG_GRAPHICS:
		lfile = FileSystem::instance().openFile(FS_SDMC, FileSystem::instance().getDataDirName() + "/logGraphics.log");
		break;
	case LOG_LOGIC:
		lfile = FileSystem::instance().openFile(FS_SDMC, FileSystem::instance().getDataDirName() + "/logLogic.log");
		break;
	default:
		Error::throwError("Attempted to log an unimplemented log type.");
		return;
		// halt exec
	}
	fseek(lfile, 0, SEEK_END);
	fwrite((data).c_str(), sizeof(char), data.size(), lfile);
	fwrite(std::string("\n").c_str(), sizeof(char), 1, lfile);
	fclose(lfile);
	// done
}

Log Log::instance_;

Log &Log::instance()
{
	return instance_;
}
