#pragma once

namespace World {
	enum WorldRotation {
		WR_NORTH,
		WR_EAST,
		WR_SOUTH,
		WR_WEST
	};

	/* Stores the current world rotation state */
	extern WorldRotation worldRotationState;

	/* Stores the current world rotatation in radians [-pi/pi] */
	extern float currentWorldRotation;
	extern float targetWorldRotation;

	/* Lerps the current world rotation to where it should be (call every frame) */
	extern void updateWorldRotation();

	/* Rotates the world state, if direction is true, go right, otherwise go left*/
	extern void rotateWorld(bool direction);

	/* Gets the radian rotation value from a rotation state */
	extern float radiansFromState(WorldRotation wr);
}