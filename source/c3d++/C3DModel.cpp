#include <c3d++/C3DModel.h>

C3DModel::C3DModel() {
}

C3DModel::C3DModel(C3DMesh mesh) :mesh_(mesh) {
}

C3DModel::C3DModel(C3DMesh mesh, C3DTexture texture) : mesh_(mesh), texture_(texture) {
}


void C3DModel::setMesh(C3DMesh mesh) {
	mesh_ = mesh;
}

C3DMesh& C3DModel::getMesh() {
	return mesh_;
}

void C3DModel::setTexture(C3DTexture texture) {
	texture_ = texture;
}

C3DTexture& C3DModel::getTexture() {
	return texture_;
}