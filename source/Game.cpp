#include <Game.h>

Game::Game()
{
	C3DRenderer::initRenderer();
}

Game::~Game()
{
}

Game Game::instance_;

Game& Game::instance() {
	return instance_;
}

void Game::run() {
	

	// load a texture
	C3DTexture homeTexture = Loader2::loadTexture("romfs:assets/textures/Home.png");
	C3DTexture skyTexture = Loader2::loadTexture("romfs:assets/textures/BackgroundDay.png");

	// load an OBJ file
	C3DMesh homeMesh = Loader2::loadOBJ("romfs:/assets/models/Home.obj");
	C3DMesh skyMesh = Loader2::loadOBJ("romfs:/assets/models/BackgroundDay.obj");

	// Create a model
	C3DModel homeModel(homeMesh, homeTexture);
	C3DModel skyModel(skyMesh, skyTexture);

	// Create and set some transforms
	C3DTransform skyTransform;
	skyTransform.setPos({ 0,0,-4 });
	skyTransform.setScale(1);

	C3DTransform homeTransform;
	homeTransform.setPos({ 0, -6, -16 });

	// Create some models
	Entity home(homeModel, homeTransform);



	while (aptMainLoop()) {

		/*
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

		rot.y = World::currentWorldRotation;
		t.setEulerRotation(rot);
		t.setPos(pos);
		homeVillage.setTransform(t);

		World::updateWorldRotation();

		Globals::renderManager_.beginFrame();
		for (unsigned int i = 0; i < backgroundEntities.size(); i++) {
			if (backgroundEntities.at(i)->hasComponent(RenderComponent::typeName)) {
				((RenderComponent*)backgroundEntities.at(i)->getComponent(RenderComponent::typeName))->receive(*backgroundEntities.at(i), Component::MessageType::MSG_RENDER);
			}
		}

		Globals::renderManager_.finishRenderLayer();
		for (unsigned int i = 0; i < worldEntities.size(); i++){
			if(worldEntities.at(i)->hasComponent(RenderComponent::typeName)){
				((RenderComponent*)worldEntities.at(i)->getComponent(RenderComponent::typeName))->receive(*worldEntities.at(i),Component::MessageType::MSG_RENDER);
			}
		}
		Globals::renderManager_.finishRenderLayer();

		for (unsigned int i = 0; i < foregroundEntities.size(); i++) {
			if (foregroundEntities.at(i)->hasComponent(RenderComponent::typeName)) {
				((RenderComponent*)foregroundEntities.at(i)->getComponent(RenderComponent::typeName))->receive(*foregroundEntities.at(i), Component::MessageType::MSG_RENDER);
			}
		}
		*/

		C3DTransform t;
		t.setPos(vec3f(0, -6, -16));

		C3DRenderer::beginFrame();
		C3DRenderer::setTarget(C3DRenderTarget::TOP);
		C3DRenderer::draw(skyModel, skyTransform);
		C3DRenderer::nextLayer();
		C3DRenderer::draw(homeModel, homeTransform);
		C3DRenderer::endFrame();
		/*

		Globals::renderManager_.endFrame();
		Globals::log_.profileEnd(__func__);
	*/
	}

}
