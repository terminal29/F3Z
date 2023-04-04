#include <c3d++/C3DModel.h>

C3DModel::C3DModel()
{
}

C3DModel::C3DModel(C3DMesh mesh) : mesh_(mesh)
{
}

C3DModel::C3DModel(C3DMesh mesh, C3DTexture texture) : mesh_(mesh), texture_(texture)
{
}

void C3DModel::setMesh(C3DMesh mesh)
{
	mesh_ = mesh;
}

C3DMesh const &C3DModel::getMesh() const
{
	return mesh_;
}

void C3DModel::setTexture(C3DTexture texture)
{
	texture_ = texture;
}

C3DTexture const &C3DModel::getTexture() const
{
	return texture_;
}