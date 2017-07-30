#pragma once

#include "Error.h"
#include <3ds.h>
#include <fstream>
#include <string>
#include <string.h> /* for strerror */

enum Drive {
	FS_SDMC,
	FS_ROMFS
};

class FileSystem
{
public:

	static FileSystem& instance();

	/* Open a filestream on the FileSystem in ab+ */
	FILE* openFile(Drive drive, std::string path);
	
	/* Open a filestream on the FileSystem */
	FILE* openFile(Drive drive, std::string path, std::string flags);
	
	/* returns the directory name for the games data store on the SD card, preceded by a / */
	std::string getDataDirName();
private:
	static FileSystem instance_;
	FileSystem();
	~FileSystem();
};

