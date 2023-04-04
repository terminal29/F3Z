#include <Game.h>
#include <memory>

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

Game &Game::instance()
{
	return instance_;
}

void Game::run()
{
	consoleInit(gfxScreen_t::GFX_BOTTOM, consoleGetDefault());
	std::cout << "Loading..." << std::endl;

	// load a texture
	C3DTexture homeTexture = Loader2::loadTexture("romfs:assets/textures/Home.png");
	C3DTexture skyTexture = Loader2::loadTexture("romfs:assets/textures/Skybox1.png");
	C3DTexture tileset1 = Loader2::loadTexture("romfs:assets/textures/tileset1.png");

	// load an OBJ file
	C3DMesh homeMesh = Loader2::loadOBJ("romfs:/assets/models/Home.obj");
	C3DMesh skyMesh = Loader2::loadOBJ("romfs:/assets/models/Skybox.obj");
	C3DMesh planeMesh = Loader2::loadOBJ("romfs:/assets/models/Plane.obj");

	// Load an island voxel tensor
	VoxelTensor vt("romfs:/islands/home.json");

	// Create a model
	C3DModel homeModel(homeMesh, homeTexture);
	C3DModel skyModel(skyMesh, skyTexture);

	C3DModel beaconModel = vt.getModel();

	// Create and set some transforms
	C3DTransform skyTransform;
	skyTransform.setPos({0, 0, 0});
	skyTransform.setScale(10);

	C3DTransform beaconTransform;
	beaconTransform.setPos({0, 0, 0});

	// Create some models
	Entity sky(skyModel, skyTransform);
	Entity beaconIsland(beaconModel, beaconTransform);

	Entity camera;
	C3DTransform cameraTransform;
	cameraTransform.setPos({0, 0, 10});
	camera.setTransform(cameraTransform);

	camera.addComponent(CameraController{true});
	sky.addComponent(RenderComponent{C3DRenderTarget::TOP, true});
	beaconIsland.addComponent(RenderComponent{C3DRenderTarget::TOP});

	std::vector<Entity *> entities = {&camera, &sky, &beaconIsland};

	std::vector<Entity *> backgroundLayer = {&sky};
	std::vector<Entity *> midLayer = {&beaconIsland};
	std::vector<Entity *> foregroundLayer;

	consoleClear();
	while (aptMainLoop())
	{
		Time::step();
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;

		// Lock skybox to camera
		auto skyTransform = sky.getTransform();
		skyTransform.setYPR(skyTransform.getYPR() + vec<float, 3>(-0.001f, 0, 0));
		skyTransform.setPos(camera.getTransform().getPos());
		sky.setTransform(skyTransform);
		auto beaconTransform = beaconIsland.getTransform();
		beaconTransform.setPos(vec<float, 3>{0, (float)std::cos(Time::t() / 100.0), 0});
		beaconIsland.setTransform(beaconTransform);

		for (Entity *entity : entities)
		{
			entity->receive(MessageType::MSG_UPDATE);
		}

		C3DRenderer::beginFrame();
		for (Entity *entity : backgroundLayer)
		{
			entity->receive(MessageType::MSG_RENDER);
		}
		C3DRenderer::nextLayer();
		for (Entity *entity : midLayer)
		{
			entity->receive(MessageType::MSG_RENDER);
		}
		C3DRenderer::nextLayer();
		for (Entity *entity : foregroundLayer)
		{
			entity->receive(MessageType::MSG_RENDER);
		}
		C3DRenderer::endFrame();
	}
}
