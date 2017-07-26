#pragma once

#include "../Error.h"
#include <citro3d.h>
#include <3ds.h>

//Shaders
#include "v_default_shbin.h"

#ifdef __INTELLISENSE__
// My intellisense keeps forgetting C3D_RT exists because reasons...
#define C3D_RenderTarget C3D_RenderTarget_tag
#endif

enum RenderTarget {
	RT_TOPLEFT,
	RT_TOPRIGHT,
	RT_BOTTOM
};

class RenderManager
{
public:
	static RenderManager& instance();
	C3D_RenderTarget* getRenderTarget(RenderTarget target);

private:
	RenderManager();
	~RenderManager();

	static RenderManager instance_;

	shaderProgram_s defaultShaderProgram_;
	C3D_RenderTarget* targetTopLeft_, *targetTopRight_, *targetBottom_;
	
	int clearColor_ = 0x68B0D8FF;

	int renderFlags_ =
		(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
		GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
		GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO)),

		textureConvertFlags_ =
		(GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
		GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
		GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));


};
