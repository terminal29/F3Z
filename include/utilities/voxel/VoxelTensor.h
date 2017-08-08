#pragma once

#include <string>
#include <fstream>
#include <vector>

#include <library\json\json.h>
#include <library\linalg.h>

#include <c3d++/C3DModel.h>
#include <Loader.h>
#include <Error.h>
#include <Log.h>

const size_t tensorSize = 32;

class VoxelType {
public:
	C3DTexture texture;
	std::string typeName;
};

class VoxelTensor {
public:
	VoxelTensor();
	VoxelTensor(std::string filepath);
	~VoxelTensor();

	void loadFromFile(std::string filepath);
	C3DModel getModel();

private:
	// elem.x,y,z position with w as the uint voxelType
	std::vector<linalg::vec<int,4>> array_;
	std::vector<VoxelType> types_;
	C3DModel model_;


	void createMesh();
};