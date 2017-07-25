#pragma once
#include "SceneNode.h"
#include "../Render/IRenderable.h"
#include "../Util.h"
#include "../Lib/picopng.h"

#include <3ds.h>
#include <citro3d.h>
#include <memory.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>


/*
	A simple node that displays text on a screen
*/

typedef struct TextNodeOpts {
	int screen = GFX_BOTTOM; // Screen to render to
	int pos[2] = { 0, 0 }; // Position on screen
	std::string characterset = "romfs:/assets/charactersets/english.png"; // Default characterset file
	std::string text = "Sample Text"; // Default text
}TextNodeOpts;

class TextNode :public SceneNode{
public:
	TextNode(SceneNode* parent, TextNodeOpts opts);
	virtual ~TextNode();
	virtual NodeType::NodeType getNodeType() override;
	virtual void update() override;

protected:
	NodeType::NodeType type = NodeType::TextNode;
	TextNodeOpts options;

	util::vertex vertex_list[6] =
	{
		{ { -0.5f, -0.5f, +0.5f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, +1.0f } },
		{ { +0.5f, -0.5f, +0.5f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, +1.0f } },
		{ { +0.5f, +0.5f, +0.5f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, +1.0f } },
		{ { +0.5f, +0.5f, +0.5f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, +1.0f } },
		{ { -0.5f, +0.5f, +0.5f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, +1.0f } },
		{ { -0.5f, -0.5f, +0.5f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, +1.0f } }
	}; // A square, will generate this dynamically...

	C3D_Tex tiledTexture;

};

