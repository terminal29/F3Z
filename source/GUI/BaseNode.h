#pragma once
#include "SceneNode.h"

class BaseNode : public SceneNode{
public:
	BaseNode();
	~BaseNode();
	virtual void update() override;
	virtual NodeType::NodeType getNodeType() override;
protected:
	NodeType::NodeType type = NodeType::BaseNode;
};

