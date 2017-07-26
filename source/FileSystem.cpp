#include "FileSystem.h"

FileSystem::FileSystem()
{
	Result rInit = romfsInit();
	if (rInit != 0) {
		Error::throwError("Cannot init Romfs!");
		//exec stop
	}
}

std::ifstream* FileSystem::openFile(FS_DRIVE drive, std::string path) {
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
		Error::throwError("Tried to open file on unknown drive.");
	}
	if (!*fs) {
		delete fs;
		Error::throwError("Cannot open file <" + drive + path + ">");
	}
	return fs;
}


FileSystem::~FileSystem()
{
	romfsExit();
}
