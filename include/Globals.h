#pragma once

/* I know this is horrendous but I dont know what to do about it other than this :(*/
#include "Game.h"
#include "Audio.h"
#include "Log.h"
#include "FileSystem.h"
#include "RenderManager.h"
#include "World.h"

/* Forward declare all the stuff */
class Game;
class Log;
class Audio;
class FileSystem;
class RenderManager;

//Namespace for storing globals
namespace Globals {
	extern Game& theGame_;
	extern Log& log_;
	extern FileSystem& fs_;
	extern Audio& audio_;
	extern RenderManager& renderManager_;
	
}