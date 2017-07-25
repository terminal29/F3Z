#pragma once
#include <string>
#include <iostream>
#include "../Util.h"
#include "BaseNode.h"

class Screen
{
public:
	static Screen* getInstance();
	void setBaseNode(BaseNode* node);

	void setup();
	void update();
	void close();

private:
	Screen();
	~Screen();

	BaseNode* node = nullptr;
};

static Screen* w_instance = nullptr;
