#include "Game.h"

Game::Game()
{
	log_ = new Log();
	fs_ = new FileSystem();
	audio_ = new Audio();
	renderManager_ = &RenderManager::instance();
}

Game::~Game()
{
	delete log_;
	delete fs_;
	delete audio_;
	// not deleting rendermanager as it is a singleton
}

// Initialize the Game
Game Game::instance_;

void Game::run() {
	log_->writeLine(LOG_GENERAL, "TEST");
}

Game& Game::instance() {
	return instance_;
}

Audio& Game::getAudio() {
	return *audio_;
}

Log& Game::getLog() {
	return *log_;
}

FileSystem& Game::getFS() {
	return *fs_;
}

RenderManager& Game::getRenderManager() {
	return *renderManager_;
}