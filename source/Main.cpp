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
	util::loadShaders();
	Screen* scr = Screen::getInstance();
	scr->setup();
	BaseNode base;
	scr->setBaseNode(&base);
	float pos[3] = { 0.0f,0.0f,-5.0f };
	Island homeIsland(&base, pos, "romfs:assets/models/home.obj", "romfs:assets/textures/home.png");

	while (aptMainLoop()) {
		util::debug_print("FRAME");
		IRenderable::beginFrame();
		scr->update();
		IRenderable::endFrame();
	}
	
}