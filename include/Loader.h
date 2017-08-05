#pragma once

#include <string>
#include <fstream>

#include <library/lodepng.h>
#include <citro3d.h>
#include <library/json/json.h>

#include <c3d++/C3DModel.h>
#include <Types.h>
#include <Error.h>

namespace Loader2 {
	C3DTexture loadTexture(std::string filepath);
	C3DMesh loadOBJ(std::string filepath);
//	Mesh loadMesh(std::string filename);
//	VoxelTensor loadStaticVoxels(std::string filename);
}