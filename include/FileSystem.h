#pragma once

#include "Error.h"
#include <3ds.h>
#include <fstream>
#include <string>

enum Drive {
	FS_SDMC,
	FS_ROMFS
};

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	/* Open a filestream on the FileSystem*/
	std::fstream* openFile(Drive drive, std::string path);

	/* returns the directory name for the games data store on the SD card, preceded by a / */
	std::string getDataDirName();

};

