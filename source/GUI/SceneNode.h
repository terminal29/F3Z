#pragma once
#include <vector>
#include <assert.h>
#include "NodeType.h"
#include "../Util.h"

class SceneNode
{
public:
	// 
	SceneNode(SceneNode* parent);
	virtual ~SceneNode();

	// Children
	virtual SceneNode* getFirstChild();
	virtual SceneNode* getLastChild();
	virtual SceneNode* getChildAt(int index);
	virtual int getNumChildren();
	
	void addChild(SceneNode* child);
	void removeChild(SceneNode* child);

	// Parent
	SceneNode* getParent();

	// Self
	virtual void update();
	virtual NodeType::NodeType getNodeType();
	

protected:
	SceneNode* parent = nullptr;
	std::vector<SceneNode*> children;
	NodeType::NodeType type = NodeType::NullNode;
private:
	SceneNode();
	void clearRemovedChildren();
};

