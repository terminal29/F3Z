#pragma once

/* I know this is horrendous but I dont know what to do about it other than this :(*/
#include "Game.h"
#include "Audio.h"
#include "Log.h"
#include "FileSystem.h"
#include "RenderManager.h"

enum WorldRotation {
	WR_NORTH = 0,
	WR_EAST = 15707963267,
	WR_SOUTH = 31415926535,
	WR_WEST = -15707963267
};


//Namespace for storing globals
namespace Globals {
	static Game& theGame_ = Game::instance();
	static Log& log_ = Log::instance();
	static FileSystem& fs_ = FileSystem::instance();
	static Audio& audio_ = Audio::instance();
	static RenderManager& renderManager_ = RenderManager::instance();
	static volatile double worldRotationState = 0;
	static volatile float currentWorldRotation = 0;
}