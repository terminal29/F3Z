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

	std::string getDataDirName();

};

