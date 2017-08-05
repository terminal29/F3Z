#pragma once
#include <vector>
#include <3ds.h>
#include <Types.h>


struct Vertex {
	vec3f position = { 0,0,0 };
	vec2f texcoord = { 0,0 };
	vec3f normal = { 0,0,0 };
	Vertex() {};
	Vertex(vec3f position, vec2f texcoord, vec3f normal) :position(position), texcoord(texcoord), normal(normal) {};
};

class C3DMesh {
public:
	C3DMesh();
	C3DMesh(std::vector<Vertex> vertices);
	~C3DMesh();

	void setVertices(std::vector<Vertex> vertices);
	std::vector<Vertex>& getVertices();

	C3DMesh operator= (const C3DMesh& tex);
	C3DMesh(const C3DMesh& mesh);
	
	void* getVBO();
private:
	std::vector<Vertex> vertices_;
	void* VBO_ = nullptr;
	bool VBODirty_ = true;
};