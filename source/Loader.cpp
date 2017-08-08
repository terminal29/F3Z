#include <Loader.h>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "library/tinyobj_loader.h"

namespace Loader2 {

	C3DTexture loadTexture(std::string filepath) {
		std::vector<unsigned char> raw;
		unsigned int w, h;
		if (lodepng::decode(raw, w, h, filepath))
			Error::throwError("Cannot load or decode PNG file " + filepath);
		if (w != h)
			Error::throwError("Texture " + filepath + " must be square");
		std::vector<std::vector<Pixel>> bitmap;
		bitmap.reserve(h);
		for (unsigned int i = 0; i < h; i++) {
			std::vector<Pixel> row;
			row.reserve(w);
			for (unsigned int j = 0; j < w; j++) {
				// Get next pixel's RGBA values
				Pixel p{ raw.at((i*w*4) + j*4), raw.at((i * w * 4) + j * 4 + 1), raw.at((i * w * 4) + j * 4 + 2), raw.at((i * w * 4) + j * 4 + 3) };
				row.push_back(p);
			}
			bitmap.push_back(row);
		}
		C3DTexture tex(bitmap);
		return tex;
	}

	C3DMesh loadOBJ(std::string filepath) {
		C3DMesh mesh;

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str());
		if (!ret) {
			Error::throwError("Cannot open or parse OBJ file: " + filepath);
		}

		std::vector <Vertex> vertices;
		Vertex vert;
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex

					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vert.position.x = attrib.vertices[3 * idx.vertex_index + 0];
					vert.position.y = attrib.vertices[3 * idx.vertex_index + 1];
					vert.position.z = attrib.vertices[3 * idx.vertex_index + 2];
					vert.normal.x = attrib.normals[3 * idx.normal_index + 0];
					vert.normal.y = attrib.normals[3 * idx.normal_index + 1];
					vert.normal.z = attrib.normals[3 * idx.normal_index + 2];
					vert.texcoord.x = attrib.texcoords[2 * idx.texcoord_index + 0];
					vert.texcoord.y = attrib.texcoords[2 * idx.texcoord_index + 1];
					vertices.push_back(vert);
				}
				index_offset += fv;
			}
		}
		mesh.setVertices(vertices);
		return mesh;
	}
}


