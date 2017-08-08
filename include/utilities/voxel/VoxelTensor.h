#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <array>

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
	std::array<int, 2> uvPos;
	int uvSize;
};

class VoxelTensor {
public:
	VoxelTensor();
	VoxelTensor(std::string filepath);
	~VoxelTensor();

	void loadFromFile(std::string filepath);
	C3DModel getModel();

	const int tilesetSize = 64;

private:
	// 0 - x, 1 - y, 2 - z, 3 - typeIndex
	std::vector<std::array<int,4>> array_;
	std::vector<VoxelType> types_;
	C3DModel model_;


	void createMesh();
};