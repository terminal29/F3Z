#pragma once

#include "Error.h"
#include <3ds.h>
#include <fstream>
#include <string>

enum FS_DRIVE {
	FS_SDMC,
	FS_ROMFS
};

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	/* Open a filestream on the FileSystem*/
	std::ifstream* openFile(FS_DRIVE drive, std::string path);

};

