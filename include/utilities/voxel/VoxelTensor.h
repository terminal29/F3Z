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

class VoxelType
{
public:
	C3DTexture texture;
	std::string typeName;
	std::array<int, 2> uvPos;
};

const int tilesetSize_ = 128; // can fit 64 different blocks

class VoxelTensor
{
public:
	VoxelTensor();
	VoxelTensor(std::string filepath);
	~VoxelTensor();

	void loadFromFile(std::string filepath);
	C3DModel getModel();

	C3DTexture getStitchedTexture();

private:
	// 0 - x, 1 - y, 2 - z, 3 - typeIndex
	std::vector<std::tuple<int, int, int, std::size_t>> array_;
	std::vector<VoxelType> types_;
	C3DModel model_;
	C3DTexture stitchedTexture_;

	void createMesh();

	void stitchTextures();
};