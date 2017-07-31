#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

Game Game::instance_;

Game& Game::instance() {
	return instance_;
}

void Game::run() {
	 

	std::vector<StaticTile> tiles = Loader::loadStaticTiles("home");

	Bitmap 
		homeBitmap = 
		Loader::loadToBitmap("assets/textures/Home.png"),
		backgroundBitmap =  
		Loader::loadToBitmap("assets/textures/BackgroundDay.png");

	C3D_Tex 
		homeTex, 
		backgroundTex;

	Loader::bitmapToTiled(homeBitmap, &homeTex);
	Loader::bitmapToTiled(backgroundBitmap, &backgroundTex);

	Model  
		homeModel = Loader::loadOBJ("assets/models/Home.obj"),
		backgroundModel = Loader::loadOBJ("assets/models/BackgroundDay.obj");

	homeModel.setTexture(homeTex);
	backgroundModel.setTexture(backgroundTex);

	Entity 
		homeVillage(homeModel), 
		backgroundDay(backgroundModel);

	Transform t = homeVillage.getTransform();
	t.setPos(vec3f(0, -6, -16));
	homeVillage.setTransform(t);

	t = backgroundDay.getTransform();
	t.setPos(vec3f(0, 0, -4));
	t.setEulerRotation(vec3f(0, 0, 0));
	t.setScale(1);
	backgroundDay.setTransform(t);

	RenderComponent
		homeVillageRC,
		backgroundRC;

	homeVillage.addComponent(&homeVillageRC);
	backgroundDay.addComponent(&backgroundRC);

	std::vector<Entity*> 
		backgroundEntities,
		worldEntities,
		foregroundEntities;

	//worldEntities.push_back(&backgroundDay);
	//worldEntities.push_back(&homeVillage);
	
	backgroundEntities.push_back(&backgroundDay);


	while (aptMainLoop()) {
		Globals::log_.profileStart(__func__);
		/* Do everything here*/
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;

		if (kDown & KEY_L) {
			World::rotateWorld(false);
		}
		if (kDown & KEY_R) {
			World::rotateWorld(true);
		}

		/* Camera controls */
		circlePosition cP;
		hidCircleRead(&cP);
		Transform t = homeVillage.getTransform();
		vec3f rot = t.getEulerRotation();
		vec3f pos = t.getPos();
		if (cP.dy > 25 || cP.dy < - 25) {
			pos.y -= (float)cP.dy / 500;
		}
		if (cP.dx > 25 || cP.dx < -25) {
			pos.x -= (float)cP.dx / 750;
		}
		//

		rot.y = World::currentWorldRotation;
		t.setEulerRotation(rot);
		t.setPos(pos);
		homeVillage.setTransform(t);

		World::updateWorldRotation();

		Globals::renderManager_.beginFrame();
		/* Render Background */
		for (unsigned int i = 0; i < backgroundEntities.size(); i++) {
			if (backgroundEntities.at(i)->hasComponent(RenderComponent::typeName)) {
				((RenderComponent*)backgroundEntities.at(i)->getComponent(RenderComponent::typeName))->receive(*backgroundEntities.at(i), Component::MessageType::MSG_RENDER);
			}
		}

		Globals::renderManager_.finishRenderLayer();
		/* Render Entities */
		for (unsigned int i = 0; i < worldEntities.size(); i++){
			if(worldEntities.at(i)->hasComponent(RenderComponent::typeName)){
				((RenderComponent*)worldEntities.at(i)->getComponent(RenderComponent::typeName))->receive(*worldEntities.at(i),Component::MessageType::MSG_RENDER);
			}
		}
		Globals::renderManager_.finishRenderLayer();
		/* Render GUI */
		for (unsigned int i = 0; i < foregroundEntities.size(); i++) {
			if (foregroundEntities.at(i)->hasComponent(RenderComponent::typeName)) {
				((RenderComponent*)foregroundEntities.at(i)->getComponent(RenderComponent::typeName))->receive(*foregroundEntities.at(i), Component::MessageType::MSG_RENDER);
			}
		}


		Globals::renderManager_.endFrame();
		Globals::log_.profileEnd(__func__);
	}
	Globals::log_.profileOutput();

}
