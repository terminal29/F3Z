#include "entity/component/Model.h"
#include "Globals.h"

Model::Model()
{
}


Model::~Model()
{
	if (VBO_ != nullptr) {
		linearFree(VBO_);
	}
	if (vertices_ != nullptr) {
		linearFree(vertices_);
	}
}

Model::Model(const Model& old) {
	*this = old;
}

Model& Model::operator=(const Model& rhs) {
	if (this != &rhs) {
		// Copy vertices
		if (rhs.vertices_ != nullptr) {
			setVertices(rhs.vertices_, rhs.numVerts_);
		}

		// Copy texture
		setTexture(rhs.texture_);

		// Ensure new VBO is created
		setDirty();
	}
	return *this;
}

const void* Model::getVBO() {
	if (VBODirty_) {
		VBODirty_ = false;
		if (VBO_ != nullptr) {
			linearFree(VBO_); // Free old vbo
		}
		// alloc space for new vbo
		VBO_ = linearAlloc(sizeof(vertex) * numVerts_); 

		// copy new vbo
		memcpy(VBO_, vertices_, sizeof(vertex) * numVerts_);
	}
	return VBO_;
}

void Model::setDirty() {
	VBODirty_ = true;
}

void Model::setVertices(vertex* newVerts, int numVerts) {
	if (vertices_ != nullptr) {
		// Free old vertex array
		linearFree(vertices_);
	}
	// Alloc space for new verts
	vertices_ = (vertex*)linearAlloc(sizeof(vertex) * numVerts);
	//copy new verts in
	memcpy(vertices_, newVerts, sizeof(vertex) * numVerts);
	numVerts_ = numVerts;
	
	
}

const Model::vertex* Model::getVertices() {
	return vertices_;
}

int Model::getNumVertices() {
	return numVerts_;
}

void Model::setTexture(C3D_Tex texture) {
	texture_ = texture;
}

const C3D_Tex* Model::getTexture() {
	return &texture_;
}