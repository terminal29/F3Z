#pragma once

#include <array>
#include <fstream>
#include <string>
#include <vector>

#include <library\json\json.h>
#include <library\linalg.h>

#include <Error.h>
#include <Loader.h>
#include <Log.h>
#include <c3d++/C3DModel.h>

class VoxelType {
public:
    C3DTexture texture;
    std::string typeName;
    std::array<int, 2> uvPos;
};

const int tilesetSize_ = 128; // can fit 64 different blocks

class VoxelTensor {
public:
    VoxelTensor();
    VoxelTensor(std::string filepath);
    ~VoxelTensor();

    void loadFromFile(std::string filepath);
    C3DModel getModel();

    C3DTexture getStitchedTexture();

    using map_t = std::unordered_map<std::string, std::tuple<int, int, int, std::size_t>>;

private:
    // 0 - x, 1 - y, 2 - z, 3 - typeIndex
    map_t array_;
    std::vector<VoxelType> types_;
    C3DModel model_;
    C3DTexture stitchedTexture_;

    void createMesh();

    void stitchTextures();
};