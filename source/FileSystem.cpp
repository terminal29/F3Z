#include "FileSystem.h"

FileSystem::FileSystem()
{
	Result rInit = romfsInit();
	if (rInit != 0) {
		Error::throwError("Cannot init Romfs!");
		//exec stop
	}
}

std::ifstream* FileSystem::openFile(Drive drive, std::string path) {
	std::ifstream* fs;
	std::string driveName;
	switch (drive) {
	case FS_ROMFS:
		driveName = "romfs";
		fs = new std::ifstream("romfs:" + path);
		break;
	case FS_SDMC:
		driveName = "sdmc";
		fs = new std::ifstream("sdmc:" + path);
		break;
	default:
		Error::throwError("Attempted to open file on unimplemented drive.");
	}
	if (!*fs) {
		delete fs;
		Error::throwError("Cannot open file <" + drive + path + ">");
	}
	return fs;
}

std::string FileSystem::getDataDirPath() {
	return "/F3Z_Data";
}


FileSystem::~FileSystem()
{
	romfsExit();
}
