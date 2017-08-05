#include <World.h>

const float PI = 3.1415926535f;

namespace World {
	WorldRotation worldRotationState = WR_NORTH;
	float currentWorldRotation = radiansFromState(worldRotationState);
	float targetWorldRotation = currentWorldRotation;

	void rotateWorld(bool right) {
		switch (worldRotationState) {
		case WR_NORTH:
			worldRotationState = right ? WR_EAST : WR_WEST;
			break;
		case WR_EAST:
			worldRotationState = right ? WR_SOUTH : WR_NORTH;
			break;
		case WR_SOUTH:
			worldRotationState = right ? WR_WEST : WR_EAST;
			break;
		case WR_WEST:
			worldRotationState = right ? WR_NORTH : WR_SOUTH;
		}
		targetWorldRotation = radiansFromState(worldRotationState);
	}

	void updateWorldRotation() {
		currentWorldRotation = targetWorldRotation;
		
	}

	float radiansFromState(WorldRotation wr) {
		switch (wr) {
		case WR_NORTH:
			return 0;
		case WR_EAST:
			return PI / 2;
		case WR_WEST:
			return (3.0f / 2) * PI;
		case WR_SOUTH:
			return PI;
		}
		return 0;
	}
}