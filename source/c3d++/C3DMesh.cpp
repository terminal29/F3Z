#include <c3d++\C3DMesh.h>
#include <cstring>

C3DMesh::C3DMesh()
{
}

C3DMesh::C3DMesh(std::vector<Vertex> vertices)
{
	setVertices(vertices);
}

C3DMesh::~C3DMesh()
{
	freeVBO();
}

void C3DMesh::setVertices(std::vector<Vertex> vertices)
{
	vertices_ = std::move(vertices);
	VBODirty_ = true;
}

std::vector<Vertex> const &C3DMesh::getVertices() const
{
	return vertices_;
}

C3DMesh::C3DMesh(const C3DMesh &that)
{
	VBODirty_ = true;
	vertices_ = that.vertices_;
}

C3DMesh C3DMesh::operator=(const C3DMesh &that)
{
	if (this != &that)
	{
		VBODirty_ = true;
		vertices_ = that.vertices_;
	}
	return *this;
}

std::span<float> C3DMesh::getVBO() const
{
	if (VBODirty_)
	{
		VBODirty_ = false;
		freeVBO();

		// alloc space for new vbo
		constexpr int stride = decltype(vertices_)::value_type::stride;
		constexpr std::size_t value_size = sizeof(decltype(vertices_)::value_type::value_type);
		const std::size_t span_length = vertices_.size() * stride;
		VBO_ = std::span<float>(static_cast<float *>(linearAlloc(value_size * span_length)), span_length);

		// Copy vertices to vbo
		auto vbo_iter = VBO_.begin();
		for (auto const &vertex : vertices_)
		{
			auto vertex_array = vertex.asArray();
			std::copy(vertex_array.begin(), vertex_array.end(), vbo_iter);
			std::advance(vbo_iter, vertex_array.size());
		}
	}
	return VBO_;
}

void C3DMesh::freeVBO() const
{
	if (!VBO_.empty())
	{
		linearFree(VBO_.data());
	}
	VBO_ = std::span<float>();
}