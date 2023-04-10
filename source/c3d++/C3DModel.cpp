#include <c3d++/C3DModel.h>

C3DModel::C3DModel()
{
}

C3DModel::C3DModel(C3DMesh mesh)
    : mesh_(mesh)
{
}

C3DModel::C3DModel(C3DMesh mesh, C3DTexture texture)
    : mesh_(mesh)
    , texture_(texture)
{
}

C3DModel::C3DModel(C3DMesh mesh, C3DTexture texture, std::shared_ptr<C3DShader> shader)
    : mesh_(mesh)
    , texture_(texture)
    , shader_(shader)
{
}

void C3DModel::setMesh(C3DMesh mesh)
{
    mesh_ = mesh;
}

C3DMesh const& C3DModel::getMesh() const
{
    return mesh_;
}

void C3DModel::setTexture(C3DTexture texture)
{
    texture_ = texture;
}

C3DTexture const& C3DModel::getTexture() const
{
    return texture_;
}

void C3DModel::setShader(std::shared_ptr<C3DShader> shader)
{
    shader_ = shader;
}

std::shared_ptr<C3DShader> C3DModel::getShader() const
{
    return shader_;
}