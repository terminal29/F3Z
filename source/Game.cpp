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
	// not deleting rendermanager as it is a singleton and will delete itself
}

// Initialize the Game
Game Game::instance_;

void Game::run() {
	log_->writeLine(LOG_GENERAL, "TEST");
	Entity testEntity;
	Model testModel;
	
	Model::vertex testVerts[6] = {
		// First triangle
		{ { -0.5f, -0.5f, +0.5f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, +1.0f } },
		{ { +0.5f, -0.5f, +0.5f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, +1.0f } },
		{ { +0.5f, +0.5f, +0.5f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, +1.0f } },
		// Second triangle
		{ { +0.5f, +0.5f, +0.5f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, +1.0f } },
		{ { -0.5f, +0.5f, +0.5f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, +1.0f } },
		{ { -0.5f, -0.5f, +0.5f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, +1.0f } }
	};

	testModel.setVertices(&testVerts[0], 6);

	RenderComponent rc;
	testEntity.addComponent(&rc);

	RenderComponent* rc_;
	if (testEntity.hasComponent(RenderComponent::typeName)) {
		rc_ = (RenderComponent*)testEntity.getComponent(RenderComponent::typeName);
	}

	while (aptMainLoop()) {
		
	}
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