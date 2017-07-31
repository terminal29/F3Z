#pragma once

#include <vector>
#include "Globals.h"
#include "loaders/Loader.h"
#include "entity\Entity.h"
#include "entity\component\RenderComponent.h"

class Game
{
public:
	static Game& instance();
	void run();

private:
	static Game instance_;
	Game();
	~Game();

};

