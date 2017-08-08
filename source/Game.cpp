#include <Game.h>

Game::Game()
{
	C3DRenderer::initRenderer();
	FileSystem::instance();
	Log::instance();
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
	C3DTexture skyTexture = Loader2::loadTexture("romfs:assets/textures/Skybox1.png");
	C3DTexture brownBrickTexture = Loader2::loadTexture("romfs:assets/textures/purpleBrick.png");

	// load an OBJ file
	C3DMesh homeMesh = Loader2::loadOBJ("romfs:/assets/models/Home.obj");
	C3DMesh skyMesh = Loader2::loadOBJ("romfs:/assets/models/Skybox.obj");

	// Load an island voxel tensor
	VoxelTensor vt("romfs:/islands/home.json"); 
	
	// Create a model
	C3DModel homeModel(homeMesh, homeTexture);
	C3DModel skyModel(skyMesh, skyTexture);
	
	C3DModel generatedModel = vt.getModel();
	generatedModel.setTexture(brownBrickTexture);
	
	// Create and set some transforms
	C3DTransform skyTransform;
	skyTransform.setPos({ 0,0,0 });
	skyTransform.setScale(10);

	C3DTransform homeTransform;
	homeTransform.setPos({ 0,0,0 });

	// Create some models
//	Entity home(homeModel, homeTransform);
	Entity sky(skyModel, skyTransform);
	Entity testIsland(generatedModel, homeTransform);

	// Set the camera to this transform
	C3DTransform camera;
	camera.setPos(vec<float,3>(0, 0, 10));
	C3DRenderer::setCameraTransform(camera);

	//home.addComponent(new RenderComponent(C3DRenderTarget::TOP));
	sky.addComponent(new RenderComponent(C3DRenderTarget::TOP, true));
	testIsland.addComponent(new RenderComponent(C3DRenderTarget::TOP));
	 
	std::vector<Entity*> backgroundLayer;
	std::vector<Entity*> midLayer;
	std::vector<Entity*> foregroundLayer;

	backgroundLayer.push_back(&sky);
//	midLayer.push_back(&home);
	midLayer.push_back(&testIsland);
	 
	consoleInit(gfxScreen_t::GFX_BOTTOM, consoleGetDefault());
	int t;
	while (aptMainLoop()) {
		t++;

		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		if (kDown & KEY_START)
			break;

		circlePosition analogStick;
		hidCircleRead(&analogStick);
		
		if ((analogStick.dy > 25 || analogStick.dy < -25)) {
			camera.setYPR(camera.getYPR() + vec<float, 3>(0, analogStick.dy / 5000.0, 0));
		}
		if ((analogStick.dx > 25 || analogStick.dx < -25)) {
			camera.setYPR(camera.getYPR() + vec<float, 3>(-analogStick.dx / 5000.0, 0, 0));
		}
		
		if (kHeld & KEY_L) {
			camera.setPos(camera.getPos() + camera.getForward());
		}
		else if (kHeld & KEY_R) {
			camera.setPos(camera.getPos() - camera.getForward());
		}
		// Lock skybox to camera

		sky.getTransform().setYPR(sky.getTransform().getYPR() + vec<float, 3>(-0.001f, 0, 0));
		sky.getTransform().setPos(camera.getPos());
		testIsland.getTransform().setPos(vec<float, 3>{0, 1 * cos(t/100.0), 0});

		C3DRenderer::beginFrame();

		for (Entity* entity : backgroundLayer) {
			entity->receive(MessageType::MSG_RENDER);
		}
		C3DRenderer::nextLayer();
		for (Entity* entity : midLayer) {
			entity->receive(MessageType::MSG_RENDER);
		}
		C3DRenderer::nextLayer();
		for (Entity* entity : foregroundLayer) {
			entity->receive(MessageType::MSG_RENDER);
		}

		C3DRenderer::endFrame();

	}

}
