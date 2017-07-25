#include <3ds.h>
#include "Util.h"
#include "GUI/Screen.h"
#include "GUI/SceneNode.h"
#include "GUI/BaseNode.h"
#include "GameObjects/Island.h"
#include "Render/IRenderable.h"



int main(int argc, char **argv) {
	Result rc = romfsInit();
	util::initC3D();
	
	Screen* scr = Screen::getInstance();
	BaseNode base;
	scr->setBaseNode(&base);
	float pos[3] = { 0.0f,0.0f,-5.0f };
	Island homeIsland(&base, pos, "romfs:assets/models/Home.obj", "romfs:assets/textures/Home.png");

	util::loadShaders();
	C3D_CullFace(GPU_CULLMODE::GPU_CULL_NONE);

	while (aptMainLoop()) {
		IRenderable::beginFrame();
		scr->update();
		IRenderable::endFrame();
	}
	
}