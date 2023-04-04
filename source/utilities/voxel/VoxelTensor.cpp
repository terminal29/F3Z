#include <utilities\voxel\VoxelTensor.h>

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
		if (!islandFileStream)
		{
			Error::throwError("Cannot load island file: " + filepath);
		}

		islandFileStream.seekg(0, std::ios::end);
		islandFileString.reserve(islandFileStream.tellg());
		islandFileStream.seekg(0, std::ios::beg);

		islandFileString.assign((std::istreambuf_iterator<char>(islandFileStream)), std::istreambuf_iterator<char>());
		Json::Value root;
		Json::Reader reader;
		if (!reader.parse(islandFileString, root))
		{
			Error::throwError("Cannot parse JSON from " + filepath + "\n" + reader.getFormattedErrorMessages());
		}
		bool valid = false;
		if (root.isMember("voxels"))
		{
			if (root["voxels"].isMember("static"))
			{
				voxelTypes = root["voxels"]["static"];
			}
			else
			{
				Error::throwError("JSON file " + filepath + " \"voxels\" has no \"static\" object. ");
			}
		}
		else
		{
			Error::throwError("JSON file " + filepath + " has no \"voxels\" object. ");
		}

		std::string voxelDataFileString;
		std::ifstream voxelDataFileStream("romfs:/config/voxelData.json");
		if (!voxelDataFileStream)
		{
			Error::throwError("Cannot load voxel data file: romfs:/config/voxelData.json");
		}

		voxelDataFileStream.seekg(0, std::ios::end);
		voxelDataFileString.reserve(voxelDataFileStream.tellg());
		voxelDataFileStream.seekg(0, std::ios::beg);

		voxelDataFileString.assign((std::istreambuf_iterator<char>(voxelDataFileStream)), std::istreambuf_iterator<char>());
		if (!reader.parse(voxelDataFileString, root))
		{
			Error::throwError("Cannot parse JSON from romfs:/config/voxelData.json\n" + reader.getFormattedErrorMessages());
		}
		valid = false;
		if (root.isMember("static"))
		{
			voxelData = root["static"];
		}
		else
		{
			Error::throwError("JSON file romfs:/config/voxelData.json has no \"static\" object.");
		}
	}

	// For each voxel type
	for (std::string voxelType : voxelTypes.getMemberNames())
	{

		if (!voxelData.isMember(voxelType))
		{
			Error::throwError(voxelType + " does not name a valid voxel type.");
		}

		if (!voxelData[voxelType].isMember("texPath") || !voxelData[voxelType]["texPath"].isString())
		{
			Error::throwError(voxelType + " does not have a valid \"texPath\"");
		}

		if (!voxelData[voxelType].isMember("uvPos") || !voxelData[voxelType]["uvPos"].isArray() || voxelData[voxelType]["uvPos"].size() != 2)
		{
			Error::throwError(voxelType + " does not have a valid \"uvPos\"");
		}

		VoxelType newType;
		newType.texture = Loader2::loadTexture(voxelData[voxelType]["texPath"].asString());
		newType.typeName = voxelType;
		newType.uvPos = std::array<int, 2>{voxelData[voxelType]["uvPos"][0].asInt(), voxelData[voxelType]["uvPos"][1].asInt()};

		// for each position
		for (Json::Value position : voxelTypes[voxelType])
		{
			std::tuple<int, int, int, std::size_t> voxelInstance = {position[0].asInt(), position[1].asInt(), position[2].asInt(), types_.size()}; // TODO add to voxelTypes if not in already
			array_.push_back(voxelInstance);
		}
		types_.push_back(newType);
	}

	// MUST BE CALLED BEFORE MESH CREATION
	stitchTextures();

	createMesh();
}

enum class Side
{
	TOP,
	BOTTOM,
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

std::vector<Vertex> createFace(Side side)
{
	std::vector<Vertex> face;
	Vertex v1, v2, v3, v4;

	switch (side)
	{
	case Side::TOP:
		v1 = Vertex({-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f});
		v2 = Vertex({-0.5f, +0.5f, +0.5f}, {1.0f, 0.0f}, {0.0f, +1.0f, 0.0f});
		v3 = Vertex({+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f});
		v4 = Vertex({+0.5f, +0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, +1.0f, 0.0f});
		break;
	case Side::BOTTOM:
		v1 = Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f});
		v2 = Vertex({+0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f});
		v3 = Vertex({+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f});
		v4 = Vertex({-0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f});
		break;
	case Side::LEFT:
		v1 = Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f});
		v2 = Vertex({-0.5f, -0.5f, +0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f});
		v3 = Vertex({-0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f});
		v4 = Vertex({-0.5f, +0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f});
		break;
	case Side::RIGHT:
		v1 = Vertex({+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f});
		v2 = Vertex({+0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {+1.0f, 0.0f, 0.0f});
		v3 = Vertex({+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f});
		v4 = Vertex({+0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {+1.0f, 0.0f, 0.0f});
		break;
	case Side::FRONT:
		v1 = Vertex({-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f});
		v2 = Vertex({+0.5f, -0.5f, +0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, +1.0f});
		v3 = Vertex({+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f});
		v4 = Vertex({-0.5f, +0.5f, +0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, +1.0f});
		break;
	case Side::BACK:
		v1 = Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f});
		v2 = Vertex({-0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f});
		v3 = Vertex({+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f});
		v4 = Vertex({+0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f});
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

std::vector<Vertex> createCube(int x, int y, int z, float uv_x, float uv_y, float uv_w, float uv_h)
{
	std::vector<Vertex> vertices;

	std::vector<Vertex> vTop = createFace(Side::TOP);
	std::vector<Vertex> vBottom = createFace(Side::BOTTOM);

	std::vector<Vertex> vLeft = createFace(Side::LEFT);
	std::vector<Vertex> vRight = createFace(Side::RIGHT);

	std::vector<Vertex> vFront = createFace(Side::FRONT);
	std::vector<Vertex> vBack = createFace(Side::BACK);

	vertices.reserve(vTop.size() + vBottom.size() + vLeft.size() + vRight.size() + vFront.size() + vBack.size());
	vertices.insert(vertices.end(), std::make_move_iterator(vTop.begin()), std::make_move_iterator(vTop.end()));
	vertices.insert(vertices.end(), std::make_move_iterator(vBottom.begin()), std::make_move_iterator(vBottom.end()));
	vertices.insert(vertices.end(), std::make_move_iterator(vLeft.begin()), std::make_move_iterator(vLeft.end()));
	vertices.insert(vertices.end(), std::make_move_iterator(vRight.begin()), std::make_move_iterator(vRight.end()));
	vertices.insert(vertices.end(), std::make_move_iterator(vFront.begin()), std::make_move_iterator(vFront.end()));
	vertices.insert(vertices.end(), std::make_move_iterator(vBack.begin()), std::make_move_iterator(vBack.end()));

	for (auto &vertex : vertices)
	{
		vertex.position.x += x;
		vertex.position.y += y;
		vertex.position.z += z;
		vertex.texcoord.x = vertex.texcoord.x * uv_w + uv_x;
		vertex.texcoord.y = vertex.texcoord.y * uv_h + uv_y;
	}

	return vertices;
}

void VoxelTensor::createMesh()
{
	C3DMesh mesh;
	std::vector<Vertex> vertices;
	for (auto const &voxel : array_)
	{
		// Create cube
		std::vector<Vertex> voxelVerts = createCube(std::get<0>(voxel), std::get<1>(voxel), std::get<2>(voxel), (float)types_.at(std::get<3>(voxel)).uvPos[0] / tilesetSize_, (float)types_.at(std::get<3>(voxel)).uvPos[1] / tilesetSize_, 16.0 / tilesetSize_, 16.0 / tilesetSize_);
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

	// Too big to fit on stack so push to heap w/ new
	std::array<std::array<Pixel, tilesetSize_>, tilesetSize_> *stitchedBitmap = new std::array<std::array<Pixel, tilesetSize_>, tilesetSize_>;
	for (std::array<Pixel, tilesetSize_> &row : *stitchedBitmap)
	{
		for (Pixel &p : row)
		{
			p.a = rand() % 255;
			p.r = rand() % 255;
			p.g = rand() % 255;
			p.b = rand() % 255;
		}
	}

	int texIndex = 0;
	if (types_.size() > 64)
	{
		Error::throwError("An island must have 64 or less different tiles");
	}

	for (VoxelType &type : types_)
	{
		std::vector<std::vector<Pixel>> bitmap = type.texture.getTexture();
		std::array<std::array<Pixel, 16>, 16> tileImg;
		int xoffs = floor((float)texIndex / tilesetSize_) * 16;
		int yoffs = (texIndex * 16) % tilesetSize_;
		// cut sub-image out of full texture
		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{
				stitchedBitmap->at(x + xoffs).at(y + yoffs) = bitmap.at(type.uvPos[1] + x).at(type.uvPos[0] + y);
			}
		}

		int uvX = yoffs;
		int uvY = 112 - xoffs;

		Log::instance().writeLine(Logfile::LOG_GENERAL, ">  " + type.typeName + " >> " + std::to_string(uvX) + " : " + std::to_string(uvY));

		type.uvPos = {uvX, uvY};
		texIndex++;
	}

	std::vector<std::vector<Pixel>> texVec;
	for (std::array<Pixel, tilesetSize_> row : *stitchedBitmap)
	{
		std::vector<Pixel> rowVec;
		for (Pixel pixel : row)
		{
			rowVec.push_back(pixel);
		}
		texVec.push_back(rowVec);
	}
	stitchedTexture_.setTexture(texVec);
	model_.setTexture(stitchedTexture_);
	delete stitchedBitmap;
}

C3DTexture VoxelTensor::getStitchedTexture()
{
	return stitchedTexture_;
}