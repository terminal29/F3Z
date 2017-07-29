#pragma once

#include "Globals.h"
#include "Loader.h"
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

	inline float wrapMP(float val) {
		float v = val;
		while (v > 3.14159265) {
			v -= 2*3.14159265;
		}
		while (v < -3.1415926) {
			v += 2 * 3.1415926;
		}
		return v;
	}

	template<typename K>
	inline K lerpAngle(K u, K v, K p) {
		return u + p*wrapMP(v - u);
	}

};

