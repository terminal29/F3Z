#include "Globals.h"

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

	// Create a test entity and model
	Entity testEntity;

	Model testModel = Loader::loadOBJ("assets/models/Home.obj");
	Bitmap b = Loader::loadToBitmap("assets/textures/Home.png");
	C3D_Tex testTex;
	Loader::bitmapToTiled(b, &testTex);
	 
	testModel.setTexture(testTex);
	testEntity.setModel(testModel);

	Transform t = testEntity.getTransform();
	vec3f pos = t.getPos();
	pos.z = -16;
	pos.y = -6;
	t.setPos(pos);
	testEntity.setTransform(t);

	RenderComponent rc;
	testEntity.addComponent(&rc);
	
	while (aptMainLoop()) {
		/* Do everything here*/
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;

		if (kDown & KEY_L) {
			Globals::worldRotationState += (float)3.14159265358979/2;
		}
		if (kDown & KEY_R) {
			Globals::worldRotationState -= (float)3.14159265358979/2;
		}

		circlePosition cP;
		hidCircleRead(&cP);
		s16 a;
		Transform t = testEntity.getTransform();
		vec3f rot = t.getEulerRotation();
		vec3f pos = t.getPos();
		if (cP.dy > 50 || cP.dy < - 50) {
			pos.y -= (float)cP.dy / 500;
		}
		if (cP.dx > 50 || cP.dx < -50) {
			pos.x -= (float)cP.dx / 750;
		}

		rot.y = Globals::currentWorldRotation;
		t.setEulerRotation(rot);
		t.setPos(pos);
		testEntity.setTransform(t);

		Globals::currentWorldRotation = lerpAngle(Globals::worldRotationState, (double)Globals::currentWorldRotation, 0.7);


		/* Then call render on all gameobjects */
		Globals::renderManager_.beginFrame();
		//for (unsigned int i = 0; i < entities.size(); i++){
		//	if(entities.at(i).hasComponent(RenderComponent::typeName)){
				((RenderComponent*)testEntity.getComponent(RenderComponent::typeName))->receive(testEntity,Component::MessageType::MSG_RENDER);
		//	}
		//}
		Globals::renderManager_.endFrame();
	}
}
