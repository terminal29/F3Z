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

	Bitmap b = Loader::loadToBitmap("assets/textures/Home.png");
	C3D_Tex testTex;
	C3D_TexInit(&testTex, b.width, b.height, GPU_TEXCOLOR::GPU_RGBA8);
	Loader::bitmapToTiled(b, &testTex);

	testModel.setTexture(testTex);
	testEntity.setModel(testModel);

	

	RenderComponent rc;
	testEntity.addComponent(&rc);

	RenderComponent* rc_;
	if (testEntity.hasComponent(RenderComponent::typeName)) {
		rc_ = (RenderComponent*)testEntity.getComponent(RenderComponent::typeName);
	} // should succeed
	else {
		Error::throwError("Assertion failed: RenderComponent could not be found after adding to entity");
	}
	while (aptMainLoop()) {
		/* Do everything here*/

		Transform t = testEntity.getTransform();
		vec3f pos = t.getPos();
		pos.z = 5 * sinf(osGetTime() / 100);
		t.setPos(pos);
		testEntity.setTransform(t);

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
