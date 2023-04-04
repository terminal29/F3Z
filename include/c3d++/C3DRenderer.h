#pragma once
#include <citro3d.h>
#include <3ds.h>

#include <c3d++/C3DModel.h>
#include <c3d++/C3DTransform.h>

#include "v_default_shbin.h"

#ifdef __INTELLISENSE__
// My intellisense keeps forgetting C3D_RT exists because reasons...
#define C3D_RenderTarget C3D_RenderTarget_tag
#endif

enum class C3DRenderTarget
{
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

namespace C3DRenderer
{

	void initRenderer();

	void closeRenderer();

	void beginFrame();

	void setCameraTransform(C3DTransform cameraTransform);

	C3DTransform const &getCameraTransform();

	void endFrame();

	void setTarget(C3DRenderTarget target);

	void drawNextShadeless();

	void draw(C3DModel const &model, C3DTransform const &transform);

	void draw(C3DMesh const &mesh, C3DTexture const &texture, C3DTransform const &transform);

	void nextLayer();
};