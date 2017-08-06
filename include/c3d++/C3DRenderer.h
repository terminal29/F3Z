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

enum class C3DRenderTarget {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

namespace C3DRenderer {

	void initRenderer();

	void closeRenderer();

	void beginFrame();

	void setCameraTransform(C3DTransform& cameraTransform);

	void endFrame();

	void setTarget(C3DRenderTarget target);

	void drawNextShadeless();

	void draw(C3DModel& model, C3DTransform& transform);

	void draw(C3DMesh& mesh, C3DTexture& texture, C3DTransform& transform);

	void nextLayer();
};