#pragma once

#include <vector>

#include <c3d++/C3DModel.h>
#include <c3d++/C3DRenderer.h>

#include <Loader.h>
#include <entity/Entity.h>
#include <entity/component/RenderComponent.h>
#include <entity/component/CameraController.h>
#include <utilities\voxel\VoxelTensor.h>

#include <utilities\Time.h>

class Game
{
public:
	static Game& instance();
	void run();

private:
	static Game instance_;
	Game();
	~Game();

};

