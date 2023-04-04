#pragma once

#include <c3d++/C3DMesh.h>
#include <c3d++/C3DTexture.h>

class C3DModel
{
public:
	C3DModel();
	C3DModel(C3DMesh mesh);
	C3DModel(C3DMesh mesh, C3DTexture texture);

	void setMesh(C3DMesh mesh);
	C3DMesh const &getMesh() const;

	void setTexture(C3DTexture texture);
	C3DTexture const &getTexture() const;

private:
	C3DMesh mesh_;
	C3DTexture texture_;
};