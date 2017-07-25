#pragma once
#include "../GUI/SceneNode.h"
#include "../Util.h"

class Island : public SceneNode
{
public:
	Island(SceneNode* parent, float* position /* x y z */, std::string modelPath, std::string texPath);
	virtual ~Island();

	virtual void update() override;
protected:
	util::vertex* model; // The islands model 
	float scale = 3;
	std::size_t numVerts;
	unsigned char* bitmap; // Texture as an RGBA texture
	C3D_Tex gpuTexture; // texture to be sent to the gpu
	float* position;
	
	NodeType::NodeType type = NodeType::Island;

	//void processTriangle(TK_TriangleVert a, TK_TriangleVert b, TK_TriangleVert c, void *userData);
};

