#include "SceneNode.h"



SceneNode::SceneNode(SceneNode* parent) {
	util::debug_print(__PRETTY_FUNCTION__);
	SceneNode::parent = parent;
	if (parent != nullptr) {
		parent->addChild(this);
	}
}

SceneNode::~SceneNode() {
	//Need to find a way to free children without causing death
}

SceneNode* SceneNode::getFirstChild() {
	return SceneNode::children.at(0);
}
SceneNode* SceneNode::getLastChild() {
	return SceneNode::children.at(SceneNode::children.size() - 1);
}
SceneNode* SceneNode::getChildAt(int index) {
	return SceneNode::children.at(index);
}

int SceneNode::getNumChildren() {
	return SceneNode::children.size();
}

SceneNode* SceneNode::getParent() {
	util::debug_print(__PRETTY_FUNCTION__);
	if (SceneNode::type == NodeType::BaseNode) {
		util::throw_error("SceneNode::getParent() -> Base Node has no parent");
	}
	else if (SceneNode::parent == nullptr) {
		util::throw_error("SceneNode::getParent() -> Node parent is nullptr");
	}
	return SceneNode::parent;
}

void SceneNode::addChild(SceneNode* child) {
	util::debug_print(__PRETTY_FUNCTION__);
	children.push_back(child);
}

void SceneNode::removeChild(SceneNode* child) {
	for (unsigned int i = 0; i < children.size(); i++) {
		if (child == children.at(i)) {
			children.at(i) = nullptr; //set to null so the next update() run will remove it.
		}
	}
}

NodeType::NodeType SceneNode::getNodeType() { 
	return SceneNode::type; 
};

void SceneNode::update() {
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children.at(i) != nullptr) {
			children.at(i)->update();
		}
	}
	SceneNode::clearRemovedChildren();
};

void SceneNode::clearRemovedChildren() {
	unsigned int idx = 0;
	while (idx < children.size()) {
		if (children.at(idx) == nullptr) {
			// Move end into the new space
			SceneNode* toSwap = children.at(children.size() - 1);
			children.at(idx) = toSwap;
			// Pop off the end
			children.pop_back();
			// Vector is now 1 less in size so our index now points to the next element
		}
		else {
			idx++;
		}
	}
}