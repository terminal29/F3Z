#include "BaseNode.h"

BaseNode::BaseNode():SceneNode(nullptr)
{
	util::debug_print("Creating " + NodeType::toString(getNodeType()));
}

BaseNode::~BaseNode()
{
	util::debug_print("Closing " + NodeType::toString(getNodeType()));
}

void BaseNode::update() {
	SceneNode::update();

	void* vbo_data = linearAlloc(sizeof(util::vertex));

	// Render some blank data so we get a blue screen even if nothing else is rendering
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vbo_data, sizeof(util::vertex), 3, 0x210);

	C3D_FrameDrawOn(util::RT_TOPLEFT);

	C3D_DrawArrays(GPU_TRIANGLES, 0, 0);

	C3D_FrameDrawOn(util::RT_TOPRIGHT);

	C3D_DrawArrays(GPU_TRIANGLES, 0, 0);

	C3D_FrameDrawOn(util::RT_BOTTOM);

	C3D_DrawArrays(GPU_TRIANGLES, 0, 0);

	linearFree(vbo_data);
	linearFree(bufInfo);

}

NodeType::NodeType BaseNode::getNodeType() {
	return BaseNode::type;
}
