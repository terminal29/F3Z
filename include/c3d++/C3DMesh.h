#pragma once
#include <vector>
#include <3ds.h>
#include <library/linalg.h>
#include <span>

using namespace linalg;

struct Vertex
{
	static constexpr std::size_t position_n = 3;
	static constexpr std::size_t texcoord_n = 2;
	static constexpr std::size_t normal_n = 3;
	static constexpr std::size_t stride = position_n + texcoord_n + normal_n;
	using value_type = float;

	Vertex(){};
	Vertex(
		vec<value_type, position_n> position,
		vec<value_type, texcoord_n> texcoord,
		vec<value_type, normal_n> normal) : position(position),
											texcoord(texcoord),
											normal(normal){};

	std::array<value_type, stride> asArray() const
	{
		return {position.x, position.y, position.z, texcoord.x, texcoord.y, normal.x, normal.y, normal.z};
	}

	vec<value_type, position_n> position = {0, 0, 0};
	vec<value_type, texcoord_n> texcoord = {0, 0};
	vec<value_type, normal_n> normal = {0, 0, 0};
};

class C3DMesh
{
public:
	C3DMesh();
	C3DMesh(std::vector<Vertex> vertices);
	~C3DMesh();

	void setVertices(std::vector<Vertex> vertices);
	std::vector<Vertex> const &getVertices();

	C3DMesh operator=(const C3DMesh &mesh);
	C3DMesh(const C3DMesh &mesh);

	std::span<float> getVBO();

private:
	void freeVBO();

	std::vector<Vertex> vertices_;
	std::span<float> VBO_;
	bool VBODirty_ = true;
};