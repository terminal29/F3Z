#pragma once

#include <citro3d.h>
#include <3ds.h>

class IRenderable
{
public:
	IRenderable() {};
	virtual ~IRenderable() {};
	virtual void render() = 0;

	static inline void beginFrame() {
		gfxSet3D(true);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	}

	static inline void endFrame() {
		C3D_FrameEnd(0);
		gspWaitForVBlank();
	}
};

