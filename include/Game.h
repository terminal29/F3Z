#pragma once

#include "render/RenderManager.h"
#include "Error.h"
#include "Log.h"
#include "FileSystem.h"
#include "Audio.h"

class Game
{
public:
	static Game& instance();

	Log& getLog();
	FileSystem& getFS();
	Audio& getAudio();
	RenderManager& getRenderManager();

	void run();
	
private:
	Game();
	~Game();

	static Game instance_;
	Log* log_;
	FileSystem* fs_;
	Audio* audio_;
	RenderManager* renderManager_;
	
};

