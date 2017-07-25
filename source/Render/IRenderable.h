#pragma once

#include <citro3d.h>

class IRenderable
{
public:
	IRenderable() {};
	virtual ~IRenderable() {};
	virtual void render() = 0;

	static inline void beginFrame() {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	}

	static inline void endFrame() {
		C3D_FrameEnd(0);
		gspWaitForVBlank();
	}
};

