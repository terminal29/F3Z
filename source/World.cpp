#include "World.h"

#define PI 3.1415926535f

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

	inline bool inRange(float val, float target, float range) {
		return (val > target - range) && (val < target + range);
	}

	void updateWorldRotation() {

			int angle_in_degrees = (( ( ( (int)(targetWorldRotation*(180/PI)) - (int)(currentWorldRotation*(180 / PI)) ) % 360) + 540) % 360) - 180;
			currentWorldRotation = angle_in_degrees * (PI/180) * 0.1f;
		
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
	}
}