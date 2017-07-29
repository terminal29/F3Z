#pragma once
#include <3ds.h>
#include <citro3d.h>

class Model
{
public:
	Model();
	~Model();

	Model(const Model& old);
	Model& operator=(const Model& rhs);

	typedef struct { float position[3]; float texcoord[2]; float normal[3]; } vertex;

	/* Gets the VBO, Creates one if it doesnt exist, call setDirty() to update the VBO */
	const void* getVBO();

	/* Sets the model dirty so the vbo is recreated next time getVBO is run */
	void setDirty();

	/* Sets the vertices to some new set of vertices and marks the vbo as dirty */
	void setVertices(vertex* newVerts, int numVerts);

	/* copies the vertices */
	const vertex* getVertices();

	int getNumVertices();

	/* Sets the texture to some new texture (Note: texture is in tiled format)*/
	void setTexture(C3D_Tex texture);

	/* Returns the current texture (Note: getTexture returns a C3D_Tex that is in tiled format so to do anything meaningful with it you will need to convert it to bitmap)*/
	const C3D_Tex* getTexture();

private:
	void* VBO_ = nullptr;
	bool VBODirty_ = true;
	C3D_Tex texture_;
	vertex* vertices_ = nullptr;
	int numVerts_ = 0;
};

