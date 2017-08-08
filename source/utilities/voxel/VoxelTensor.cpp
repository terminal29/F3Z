#include <utilities\voxel\VoxelTensor.h>

VoxelTensor::VoxelTensor() {

}

VoxelTensor::~VoxelTensor() {

}

VoxelTensor::VoxelTensor(std::string filepath) {
	loadFromFile(filepath);
}

void VoxelTensor::loadFromFile(std::string filepath) {
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
			}
			else {
				Error::throwError("JSON file " + filepath + " \"voxels\" has no \"static\" object. ");
			}
		}
		else{
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
		}
		else {
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

		if (!voxelData[voxelType].isMember("uvSize") || !voxelData[voxelType]["uvSize"].isInt()) {
			Error::throwError(voxelType + " does not have a valid \"uvSize\"");
		}

		if (!voxelData[voxelType].isMember("uvPos") || !voxelData[voxelType]["uvPos"].isArray() || voxelData[voxelType]["uvPos"].size() != 2) {
			Error::throwError(voxelType + " does not have a valid \"uvPos\"");
		}

		VoxelType newType;
		newType.texture = Loader2::loadTexture(voxelData[voxelType]["texPath"].asString());
		newType.typeName = voxelType;
		newType.uvPos = std::array<int, 2>{voxelData[voxelType]["uvPos"][0].asInt(), voxelData[voxelType]["uvPos"][1].asInt()};
		newType.uvSize = voxelData[voxelType]["uvSize"].asInt();

		// for each position
		for (Json::Value position : voxelTypes[voxelType]) {
			std::array<int, 4> voxelInstance = { position[0].asInt(), position[1].asInt(),position[2].asInt(), types_.size() }; // TODO add to voxelTypes if not in already
			array_.push_back(voxelInstance);
		}
		types_.push_back(newType);
	}

	createMesh();

}

enum class Side {
	TOP,
	BOTTOM,
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

std::vector<Vertex> createFace(Side side) {
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
	
	//first tri
	face.push_back(v1);
	face.push_back(v2);
	face.push_back(v3);

	//second tri
	face.push_back(v3);
	face.push_back(v4);
	face.push_back(v1);

	return face;
}

std::vector<Vertex> createCube(int x, int y, int z, float uv_x, float uv_y, float uv_w, float uv_h) {
	std::vector<Vertex> vertices;

	std::vector<Vertex> vTop = createFace(Side::TOP);
	std::vector<Vertex> vBottom = createFace(Side::BOTTOM);

	std::vector<Vertex> vLeft = createFace(Side::LEFT);
	std::vector<Vertex> vRight = createFace(Side::RIGHT);

	std::vector<Vertex> vFront = createFace(Side::FRONT);
	std::vector<Vertex> vBack = createFace(Side::BACK);

	vertices.reserve(vTop.size() + vBottom.size() + vLeft.size() + vRight.size() + vFront.size() + vBack.size());
	vertices.insert(vertices.end(), vTop.begin(), vTop.end());
	vertices.insert(vertices.end(), vBottom.begin(), vBottom.end());
	vertices.insert(vertices.end(), vLeft.begin(), vLeft.end());
	vertices.insert(vertices.end(), vRight.begin(), vRight.end());
	vertices.insert(vertices.end(), vFront.begin(), vFront.end());
	vertices.insert(vertices.end(), vBack.begin(), vBack.end());

	for (int i = 0; i < vertices.size(); i++) {
		vertices.at(i).position.x += x;
		vertices.at(i).position.y += y;
		vertices.at(i).position.z += z;
	
		vertices.at(i).texcoord.x *= uv_w;
		vertices.at(i).texcoord.x += uv_x;
		vertices.at(i).texcoord.y *= uv_h;
		vertices.at(i).texcoord.y += uv_y;
	}
	return vertices;
}

void VoxelTensor::createMesh() {
	C3DMesh mesh;
	std::vector<Vertex> vertices;
	for (std::array<int, 4> voxel : array_) {
		// Create cube
		std::vector<Vertex> voxelVerts = createCube(voxel[0], voxel[1], voxel[2], (float)types_.at(voxel[3]).uvPos[0] / tilesetSize, (float)types_.at(voxel[3]).uvPos[1] / tilesetSize, (float)types_.at(voxel[3]).uvSize / tilesetSize, (float)types_.at(voxel[3]).uvSize / tilesetSize);
		// alloc space 
		vertices.reserve(vertices.size() + voxelVerts.size());
		// put in
		vertices.insert(vertices.end(), voxelVerts.begin(), voxelVerts.end());
	}
	mesh.setVertices(vertices);
	model_.setMesh(mesh);
}

C3DModel VoxelTensor::getModel() {
	return model_;
}