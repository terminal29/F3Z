#include <utilities\voxel\VoxelTensor.h>

inline std::string keyOf(int x, int y, int z)
{
    return std::to_string(x) + ":" + std::to_string(y) + ":" + std::to_string(z);
}

VoxelTensor::VoxelTensor()
{
}

VoxelTensor::~VoxelTensor()
{
}

VoxelTensor::VoxelTensor(std::string filepath)
{
    loadFromFile(filepath);
}

void VoxelTensor::loadFromFile(std::string filepath)
{
    // list of each voxel type, containing the positions it should be placed in

    Json::Value voxelTypes(Json::arrayValue);
    Json::Value voxelData(Json::arrayValue);
    {
        std::string islandFileString;
        std::ifstream islandFileStream(filepath);
        if (!islandFileStream) {
            Error::throwError("Cannot load island file: " + filepath);
        }

        islandFileStream.seekg(0, std::ios::end);
        islandFileString.reserve(islandFileStream.tellg());
        islandFileStream.seekg(0, std::ios::beg);

        islandFileString.assign((std::istreambuf_iterator<char>(islandFileStream)), std::istreambuf_iterator<char>());
        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(islandFileString, root)) {
            Error::throwError("Cannot parse JSON from " + filepath + "\n" + reader.getFormattedErrorMessages());
        }
        bool valid = false;
        if (root.isMember("voxels")) {
            if (root["voxels"].isMember("static")) {
                voxelTypes = root["voxels"]["static"];
            } else {
                Error::throwError("JSON file " + filepath + " \"voxels\" has no \"static\" object. ");
            }
        } else {
            Error::throwError("JSON file " + filepath + " has no \"voxels\" object. ");
        }

        std::string voxelDataFileString;
        std::ifstream voxelDataFileStream("romfs:/config/voxelData.json");
        if (!voxelDataFileStream) {
            Error::throwError("Cannot load voxel data file: romfs:/config/voxelData.json");
        }

        voxelDataFileStream.seekg(0, std::ios::end);
        voxelDataFileString.reserve(voxelDataFileStream.tellg());
        voxelDataFileStream.seekg(0, std::ios::beg);

        voxelDataFileString.assign((std::istreambuf_iterator<char>(voxelDataFileStream)), std::istreambuf_iterator<char>());
        if (!reader.parse(voxelDataFileString, root)) {
            Error::throwError("Cannot parse JSON from romfs:/config/voxelData.json\n" + reader.getFormattedErrorMessages());
        }
        valid = false;
        if (root.isMember("static")) {
            voxelData = root["static"];
        } else {
            Error::throwError("JSON file romfs:/config/voxelData.json has no \"static\" object.");
        }
    }

    // For each voxel type
    for (std::string voxelType : voxelTypes.getMemberNames()) {

        if (!voxelData.isMember(voxelType)) {
            Error::throwError(voxelType + " does not name a valid voxel type.");
        }

        if (!voxelData[voxelType].isMember("texPath") || !voxelData[voxelType]["texPath"].isString()) {
            Error::throwError(voxelType + " does not have a valid \"texPath\"");
        }

        if (!voxelData[voxelType].isMember("uvPos") || !voxelData[voxelType]["uvPos"].isArray() || voxelData[voxelType]["uvPos"].size() != 2) {
            Error::throwError(voxelType + " does not have a valid \"uvPos\"");
        }

        VoxelType newType;
        newType.texture = Loader2::loadTexture(voxelData[voxelType]["texPath"].asString());
        newType.typeName = voxelType;
        newType.uvPos = std::array<int, 2> { voxelData[voxelType]["uvPos"][0].asInt(), voxelData[voxelType]["uvPos"][1].asInt() };

        // for each position
        for (Json::Value position : voxelTypes[voxelType]) {
            const auto x = position[0].asInt();
            const auto y = position[1].asInt();
            const auto z = position[2].asInt();
            std::tuple<int, int, int, std::size_t> voxelInstance = { x, y, z, types_.size() }; // TODO add to voxelTypes if not in already
            array_.emplace(keyOf(x, y, z), voxelInstance);
        }
        types_.push_back(newType);
    }

    // MUST BE CALLED BEFORE MESH CREATION
    stitchTextures();

    createMesh();
}

enum class Side : uint8_t {
    NONE = 0,
    TOP = (1u << 0),
    BOTTOM = (1u << 1),
    FRONT = (1u << 2),
    BACK = (1u << 3),
    LEFT = (1u << 4),
    RIGHT = (1u << 5)
};

std::vector<Vertex> createFace(Side side)
{
    std::vector<Vertex> face;
    Vertex v1, v2, v3, v4;

    switch (side) {
    case Side::TOP:
        v1 = Vertex({ -0.5f, +0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, +1.0f, 0.0f });
        v2 = Vertex({ -0.5f, +0.5f, +0.5f }, { 1.0f, 0.0f }, { 0.0f, +1.0f, 0.0f });
        v3 = Vertex({ +0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f }, { 0.0f, +1.0f, 0.0f });
        v4 = Vertex({ +0.5f, +0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f, +1.0f, 0.0f });
        break;
    case Side::BOTTOM:
        v1 = Vertex({ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f });
        v2 = Vertex({ +0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f });
        v3 = Vertex({ +0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f });
        v4 = Vertex({ -0.5f, -0.5f, +0.5f }, { 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f });
        break;
    case Side::LEFT:
        v1 = Vertex({ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f });
        v2 = Vertex({ -0.5f, -0.5f, +0.5f }, { 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f });
        v3 = Vertex({ -0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f });
        v4 = Vertex({ -0.5f, +0.5f, -0.5f }, { 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f });
        break;
    case Side::RIGHT:
        v1 = Vertex({ +0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { +1.0f, 0.0f, 0.0f });
        v2 = Vertex({ +0.5f, +0.5f, -0.5f }, { 1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f });
        v3 = Vertex({ +0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f }, { +1.0f, 0.0f, 0.0f });
        v4 = Vertex({ +0.5f, -0.5f, +0.5f }, { 0.0f, 1.0f }, { +1.0f, 0.0f, 0.0f });
        break;
    case Side::FRONT:
        v1 = Vertex({ -0.5f, -0.5f, +0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f });
        v2 = Vertex({ +0.5f, -0.5f, +0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, +1.0f });
        v3 = Vertex({ +0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, +1.0f });
        v4 = Vertex({ -0.5f, +0.5f, +0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, +1.0f });
        break;
    case Side::BACK:
        v1 = Vertex({ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
        v2 = Vertex({ -0.5f, +0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
        v3 = Vertex({ +0.5f, +0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });
        v4 = Vertex({ +0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });
        break;
    }

    // first tri
    face.push_back(v1);
    face.push_back(v2);
    face.push_back(v3);

    // second tri
    face.push_back(v3);
    face.push_back(v4);
    face.push_back(v1);

    return face;
}

std::vector<Vertex> createCube(
    int x,
    int y,
    int z,
    float uv_x,
    float uv_y,
    float uv_w,
    float uv_h,
    uint8_t skipSides = static_cast<uint8_t>(Side::NONE))
{
    static constexpr auto SIDES = { Side::TOP, Side::BOTTOM, Side::LEFT, Side::RIGHT, Side::FRONT, Side::BACK };

    std::vector<Vertex> vertices;
    std::size_t reserved = 0;

    for (const auto& side : SIDES) {
        if (!(skipSides & static_cast<uint8_t>(side))) {
            std::vector<Vertex> sideVertices = createFace(side);
            for (auto& vertex : sideVertices) {
                vertex.position.x += x;
                vertex.position.y += y;
                vertex.position.z += z;
                vertex.texcoord.x = vertex.texcoord.x * uv_w + uv_x;
                vertex.texcoord.y = vertex.texcoord.y * uv_h + uv_y;
            }
            vertices.insert(vertices.end(), std::make_move_iterator(sideVertices.begin()), std::make_move_iterator(sideVertices.end()));
        }
    }

    return vertices;
}

uint8_t getSkipMask(int x, int y, int z, const VoxelTensor::map_t& map)
{
    uint8_t skipMask = static_cast<uint8_t>(Side::NONE);

    if (map.contains(keyOf(x, y + 1, z))) {
        skipMask |= static_cast<uint8_t>(Side::TOP);
    }
    if (map.contains(keyOf(x, y - 1, z))) {
        skipMask |= static_cast<uint8_t>(Side::BOTTOM);
    }
    if (map.contains(keyOf(x - 1, y, z))) {
        skipMask |= static_cast<uint8_t>(Side::LEFT);
    }
    if (map.contains(keyOf(x + 1, y, z))) {
        skipMask |= static_cast<uint8_t>(Side::RIGHT);
    }
    if (map.contains(keyOf(x, y, z + 1))) {
        skipMask |= static_cast<uint8_t>(Side::FRONT);
    }
    if (map.contains(keyOf(x, y, z - 1))) {
        skipMask |= static_cast<uint8_t>(Side::BACK);
    }

    return skipMask;
}

void VoxelTensor::createMesh()
{
    C3DMesh mesh;
    std::vector<Vertex> vertices;
    for (auto const& voxel : array_) {
        // Create cube
        auto const& [x, y, z, t] = voxel.second;
        auto skipMask = getSkipMask(x, y, z, array_);
        std::vector<Vertex> voxelVerts = createCube(x, y, z, (float)types_.at(t).uvPos[0] / tilesetSize_, (float)types_.at(t).uvPos[1] / tilesetSize_, 16.0 / tilesetSize_, 16.0 / tilesetSize_, skipMask);
        // alloc space
        vertices.reserve(vertices.size() + voxelVerts.size());
        // put in
        vertices.insert(vertices.end(), std::make_move_iterator(voxelVerts.begin()), std::make_move_iterator(voxelVerts.end()));
    }
    mesh.setVertices(vertices);
    model_.setMesh(mesh);
}

C3DModel VoxelTensor::getModel()
{
    return model_;
}

void VoxelTensor::stitchTextures()
{
    using tileset_row_t = std::vector<Pixel>;
    using tileset_t = std::vector<tileset_row_t>;
    // Too big to fit on stack
    std::unique_ptr<tileset_t> stitchedBitmap = std::make_unique<tileset_t>(tilesetSize_, tileset_row_t(tilesetSize_, Pixel {}));
    // for (std::array<Pixel, tilesetSize_> &row : *stitchedBitmap)
    // {
    // 	for (Pixel &p : row)
    // 	{
    // 		p.a = rand() % 255;
    // 		p.r = rand() % 255;
    // 		p.g = rand() % 255;
    // 		p.b = rand() % 255;
    // 	}
    // }

    int texIndex = 0;
    if (types_.size() > 64) {
        Error::throwError("An island must have 64 or less different tiles");
    }

    for (VoxelType& type : types_) {
        auto const& bitmap = type.texture.getTexture();
        std::array<std::array<Pixel, 16>, 16> tileImg;
        int xoffs = floor((float)texIndex / tilesetSize_) * 16;
        int yoffs = (texIndex * 16) % tilesetSize_;
        // cut sub-image out of full texture
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                stitchedBitmap->at(x + xoffs).at(y + yoffs) = bitmap.at(type.uvPos[1] + x).at(type.uvPos[0] + y);
            }
        }

        int uvX = yoffs;
        int uvY = 112 - xoffs;

        Log::instance().writeLine(Logfile::LOG_GENERAL, ">  " + type.typeName + " >> " + std::to_string(uvX) + " : " + std::to_string(uvY));

        type.uvPos = { uvX, uvY };
        texIndex++;
    }

    std::vector<std::vector<Pixel>> texVec;
    for (auto const& row : *stitchedBitmap) {
        texVec.emplace_back(std::vector<Pixel> { std::begin(row), std::end(row) });
    }
    stitchedTexture_.setTexture(texVec);
    model_.setTexture(stitchedTexture_);
}

C3DTexture VoxelTensor::getStitchedTexture()
{
    return stitchedTexture_;
}