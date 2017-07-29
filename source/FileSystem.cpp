#include "Globals.h"

FileSystem::FileSystem()
{
	// Init romfs
	Result rInit = romfsInit();
	if (rInit != 0) {
		Error::throwError("Cannot init Romfs!");
		//exec stop
	}

	// Create data directories (no standard way to do this so we need 3ds specific funcs)
	
	FS_Archive SD;
	FS_Path root = fsMakePath(FS_PathType::PATH_ASCII, "");

	// Open root
	Result rs = FSUSER_OpenArchive(&SD, FS_ArchiveID::ARCHIVE_SDMC, root);
	if (rs != 0) {
		Error::throwError("Attempted to open SDMC but failed");
	}
	FS_Path dataDir = fsMakePath(FS_PathType::PATH_ASCII,std::string(getDataDirName() + "/").c_str());

	// Create data directory
	Handle dirHandle;
	rs = FSUSER_OpenDirectory(&dirHandle, SD, dataDir);
	if (rs != 0) {
		// Directory doesnt exist, so create it
		rs = FSUSER_CreateDirectory(SD, dataDir, 0);
		if (rs != 0) {
			Error::throwError("Attempted to create sdmc:" + getDataDirName() + "/ but failed");
		}
		// Check if it's been created
		rs = FSUSER_OpenDirectory(&dirHandle, SD, dataDir);
		if (rs != 0) {
			Error::throwError("Attempted to open sdmc:" + getDataDirName() + "/ but failed");
		}
		// Directory now exists
	}// Directory exists

	//Close the directory
	FSDIR_Close(dirHandle);

	//Close the archive
	FSUSER_CloseArchive(SD);

}

FILE* FileSystem::openFile(Drive drive, std::string path) {
	std::fstream* fs;
	std::string driveName;
	switch (drive) {
	case FS_ROMFS:
		driveName = "romfs:";
		break;
	case FS_SDMC:
		driveName = "sdmc:";
		break;
	default:
		Error::throwError("Attempted to open file on unimplemented drive.");
	}
	FILE* fi = fopen((driveName + path).c_str(), "ab+"); //Make sure the file exists
	if (!fi) {
		delete fi;
		Error::throwError("Cannot open file <" + driveName + path + ">");
		// exec stop
	}
	return fi;
}

std::string FileSystem::getDataDirName() {
	return "/F3Z_Data";
}


FileSystem::~FileSystem()
{
	romfsExit();
}


FileSystem FileSystem::instance_;

FileSystem& FileSystem::instance() {
	return instance_;
}

