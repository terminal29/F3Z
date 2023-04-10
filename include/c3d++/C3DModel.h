#pragma once

#include <c3d++/C3DMesh.h>
#include <c3d++/C3DShader.h>
#include <c3d++/C3DTexture.h>

class C3DModel {
public:
    C3DModel();
    C3DModel(C3DMesh mesh);
    C3DModel(C3DMesh mesh, C3DTexture texture);
    C3DModel(C3DMesh mesh, C3DTexture texture, std::shared_ptr<C3DShader> shader);

    void setMesh(C3DMesh mesh);
    C3DMesh const& getMesh() const;

    void setTexture(C3DTexture texture);
    C3DTexture const& getTexture() const;

    void setShader(std::shared_ptr<C3DShader> shader);
    std::shared_ptr<C3DShader> getShader() const;

private:
    std::shared_ptr<C3DShader> shader_;
    C3DMesh mesh_;
    C3DTexture texture_;
};