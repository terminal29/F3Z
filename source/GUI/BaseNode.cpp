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


}

NodeType::NodeType BaseNode::getNodeType() {
	return BaseNode::type;
}
