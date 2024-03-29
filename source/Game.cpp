#include <Game.h>
#include <entity/component/CharacterController.h>
#include <entity/component/RigidBodyComponent.h>
#include <entity/component/WorldComponent.h>
#include <memory>
#include <v_testing_shbin.h>

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

    C3DTexture tileset1 = Loader2::loadTexture("romfs:assets/textures/tileset1.png");

    // load an OBJ file
    C3DMesh homeMesh = Loader2::loadOBJ("romfs:/assets/models/Home.obj");

    C3DMesh planeMesh = Loader2::loadOBJ("romfs:/assets/models/Plane.obj");

    // Load an island voxel tensor

    // Create a model
    C3DModel homeModel(homeMesh, homeTexture);

    // Create and set some transforms

    baba::Entito world;
    world.setName("World");
    world.addComponent(WorldComponent {});

    // Create some models

    auto cameraEntitoId = world.addChild([] {
        baba::Entito camera;
        camera.setName("Camera");
        camera.addComponent(CameraController {});
        return camera;
    }());
    world.addChild([] {
        VoxelTensor vt("romfs:/islands/home.json");
        C3DModel beaconModel = vt.getModel();
        C3DTransform beaconTransform;
        baba::Entito beaconIsland(beaconModel, beaconTransform);
        beaconIsland.setName("Island");
        beaconIsland.addComponent(RenderComponent { C3DRenderTarget::TOP, MessageType::MSG_RENDER_MID });
        return beaconIsland;
    }());
    world.addChild([] {
        C3DTexture skyTexture = Loader2::loadTexture("romfs:assets/textures/Skybox1.png");
        C3DMesh skyMesh = Loader2::loadOBJ("romfs:/assets/models/Skybox.obj");
        C3DModel skyModel(skyMesh, skyTexture);
        C3DTransform skyTransform;
        skyTransform.setPos({ 0, 0, 0 });
        skyTransform.setScale(10);
        baba::Entito sky(skyModel, skyTransform);
        sky.setName("Sky");
        sky.addComponent(RenderComponent { C3DRenderTarget::TOP, MessageType::MSG_RENDER_BACKGROUND, true });
        return sky;
    }());
    auto characterEntitoId = world.addChild([] {
        baba::Entito gomez(C3DModel { Loader2::loadOBJ("romfs:/assets/models/Gomez.obj"), Loader2::loadTexture("romfs:/assets/textures/Gomez.png") });
        gomez.setName("Gomez");
        gomez.addComponent(RenderComponent { C3DRenderTarget::TOP, MessageType::MSG_RENDER_MID, false, true });
        gomez.addComponent(CharacterController {});
        gomez.addComponent(RigidBodyComponent {});
        return gomez;
    }());
    world.findChildByName(cameraEntitoId).lock()->getComponent<CameraController>().lock()->setFollowEntity(world.findChildByName(characterEntitoId));

    while (aptMainLoop()) {
        consoleClear();
        Time::step();
        std::cout << "Dt: " << Time::dt() << std::endl;
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break;

        world.receive(MessageType::MSG_UPDATE);

        world.receive(MessageType::MSG_PHYSICS_TICK);

        C3DRenderer::beginFrame();
        world.receive(MessageType::MSG_RENDER_BACKGROUND);

        C3DRenderer::nextLayer();
        world.receive(MessageType::MSG_RENDER_MID);

        C3DRenderer::nextLayer();
        world.receive(MessageType::MSG_RENDER_FOREGROUND);

        C3DRenderer::endFrame();
    }
}
