#pragma once
#include <vector>
#include <3ds.h>
#include <library/linalg.h>

using namespace linalg;

struct Vertex {
	vec<float,3> position = { 0,0,0 };
	vec<float,2> texcoord = { 0,0 };
	vec<float,3> normal = { 0,0,0 };
	Vertex() {};
	Vertex(vec<float, 3> position, vec<float, 2> texcoord, vec<float, 3> normal) :position(position), texcoord(texcoord), normal(normal) {};
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