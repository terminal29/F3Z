#pragma once

/* I know this is horrendous but I dont know what to do about it other than this :(*/
#include "Game.h"
#include "Audio.h"
#include "Log.h"
#include "FileSystem.h"
#include "RenderManager.h"

//Namespace for storing globals
namespace Globals {
	static Game& theGame_ = Game::instance();
	static Log& log_ = Log::instance();
	static FileSystem& fs_ = FileSystem::instance();
	static Audio& audio_ = Audio::instance();
	static RenderManager& renderManager_ = RenderManager::instance();
}