#include <c3d++\C3DMesh.h>

C3DMesh::C3DMesh() {
}

C3DMesh::C3DMesh(std::vector<Vertex> vertices) {
	vertices_ = vertices;
	VBODirty_ = true;
}

C3DMesh::~C3DMesh() {
	if (VBO_ != nullptr) {
		linearFree(VBO_);
	}
}

void C3DMesh::setVertices(std::vector<Vertex> vertices) {
	vertices_ = vertices;
	VBODirty_ = true;
}

std::vector<Vertex>& C3DMesh::getVertices() {
	return vertices_;
}

C3DMesh::C3DMesh(const C3DMesh& that) {
	VBODirty_ = true;
	vertices_ = that.vertices_;
}

C3DMesh C3DMesh::operator= (const C3DMesh& that){
	if (this != &that) {
		VBODirty_ = true;
		vertices_ = that.vertices_;
		if (VBO_ != nullptr) {
			linearFree(VBO_);
		}
	}
	return *this;
}


void* C3DMesh::getVBO() {
	if (VBODirty_) {
		VBODirty_ = false;
		if (VBO_ != nullptr) {
			linearFree(VBO_); // Free old vbo
		}
		// alloc space for new vbo
		VBO_ = linearAlloc(8 * sizeof(float) * vertices_.size());

		// copy vertex data into vbo
		for (int i = 0; i < vertices_.size(); i++) {
			((float*)VBO_)[i * 8] = vertices_.at(i).position.x;
			((float*)VBO_)[i * 8 + 1] = vertices_.at(i).position.y;
			((float*)VBO_)[i * 8 + 2] = vertices_.at(i).position.z;

			((float*)VBO_)[i * 8 + 3] = vertices_.at(i).texcoord.x;
			((float*)VBO_)[i * 8 + 4] = vertices_.at(i).texcoord.y;

			((float*)VBO_)[i * 8 + 5] = vertices_.at(i).normal.x;
			((float*)VBO_)[i * 8 + 6] = vertices_.at(i).normal.y;
			((float*)VBO_)[i * 8 + 7] = vertices_.at(i).normal.z;
		}
	}
	return VBO_;
}