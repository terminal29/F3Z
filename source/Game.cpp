#include "Game.h"

Game::Game()
{
	log_ = new Log();
	fs_ = new FileSystem();
	audio_ = new Audio();
	renderManager_ = new RenderManager();
}

Game::~Game()
{
	delete log_;
	delete fs_;
	delete audio_;
	delete renderManager_;
}

// Initialize the Game
Game Game::instance_;

void Game::run() {
	// do all the things
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