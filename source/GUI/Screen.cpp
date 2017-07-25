#include "Screen.h"

Screen* Screen::getInstance() {
	if (!w_instance) {
		w_instance = new Screen();
		util::debug_print(__PRETTY_FUNCTION__ + std::string("Screen Created"));
	}
	return w_instance;
}

void Screen::setBaseNode(BaseNode* node) {
	util::debug_print(__PRETTY_FUNCTION__);
	Screen::node = node;
}

void Screen::setup(){
	
}

void Screen::update() {
	if (node == nullptr) {
		util::debug_print("Screen::update() -> BaseNode is nullptr");
		return;
	}

	hidScanInput();
	node->update();
}

void Screen::close() {
	util::debug_print("Closing Screen");
	hidExit();
	gfxExit();
}

Screen::Screen() {
	hidInit();
	consoleDebugInit(debugDevice_SVC);
}

Screen::~Screen(){

	
}
