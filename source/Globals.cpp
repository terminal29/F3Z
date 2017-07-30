#include "Globals.h"

namespace Globals {
	Game& theGame_ = Game::instance();
	Log& log_ = Log::instance();
	FileSystem& fs_ = FileSystem::instance();
	Audio& audio_ = Audio::instance();
	RenderManager& renderManager_ = RenderManager::instance();
}