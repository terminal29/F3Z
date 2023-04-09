#include <Game.h>
#include <entity/component/WorldComponent.h>
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

Game& Game::instance()
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
    skyTransform.setPos({ 0, 0, 0 });
    skyTransform.setScale(10);

    C3DTransform beaconTransform;
    beaconTransform.setPos({ 0, 0, 0 });

    Entity world;
    world.setName("World");
    world.addComponent(WorldComponent {});

    // Create some models
    Entity sky(skyModel, skyTransform);
    sky.setName("Sky");
    Entity beaconIsland(beaconModel, beaconTransform);
    beaconIsland.setName("Island");

    Entity camera;
    camera.setName("Camera");

    camera.addComponent(CameraController {});
    sky.addComponent(RenderComponent { C3DRenderTarget::TOP, MessageType::MSG_RENDER_BACKGROUND, true });
    beaconIsland.addComponent(RenderComponent { C3DRenderTarget::TOP, MessageType::MSG_RENDER_MID });

    world.addChild(std::move(camera));
    world.addChild(sky);
    world.addChild(beaconIsland);

    while (aptMainLoop()) {
        consoleClear();
        Time::step();
        // std::cout << "Dt: " << Time::dt() << std::endl;
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
        beaconTransform.setPos(vec<float, 3> { -20, (float)std::cos(Time::t() / 100.0), -20 });
        beaconIsland.setTransform(beaconTransform);

        world.receive(MessageType::MSG_UPDATE);

        C3DRenderer::beginFrame();
        world.receive(MessageType::MSG_RENDER_BACKGROUND);

        C3DRenderer::nextLayer();
        world.receive(MessageType::MSG_RENDER_MID);

        C3DRenderer::nextLayer();
        world.receive(MessageType::MSG_RENDER_FOREGROUND);

        C3DRenderer::endFrame();
    }
}
