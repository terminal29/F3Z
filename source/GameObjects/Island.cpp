#include "Island.h"



Island::Island(SceneNode* parent, float* pos /* x y z */, std::string modelPath, std::string texPath) :SceneNode(parent),position(pos)
{
	//Load texture into bitmap

	//load bitmap into C3DTex
}

void Island::update() {
	//Render island at position
}


Island::~Island()
{
}
