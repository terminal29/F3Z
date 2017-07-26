#include "render/RenderManager.h"



RenderManager::RenderManager()
{

	// Init RenderTargets
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	targetTopLeft_ = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetClear(targetTopLeft_, C3D_CLEAR_ALL, clearColor_, 0);
	C3D_RenderTargetSetOutput(targetTopLeft_, GFX_TOP, GFX_LEFT, renderFlags_);

	targetTopRight_ = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetClear(targetTopRight_, C3D_CLEAR_ALL, clearColor_, 0);
	C3D_RenderTargetSetOutput(targetTopRight_, GFX_TOP, GFX_RIGHT, renderFlags_);

	targetBottom_ = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetClear(targetBottom_, C3D_CLEAR_ALL, clearColor_, 0);
	C3D_RenderTargetSetOutput(targetBottom_, GFX_BOTTOM, GFX_LEFT, renderFlags_);

	// Load shader (s)
	DVLB_s* vshader_dvlb;
	vshader_dvlb = DVLB_ParseFile((u32*)v_default_shbin, v_default_shbin_size);
	shaderProgramInit(&defaultShaderProgram_);
	shaderProgramSetVsh(&defaultShaderProgram_, &vshader_dvlb->DVLE[0]);
	//linearFree(vshader_dvlb); //Unsure if this is allowed

}

C3D_RenderTarget* RenderManager::getRenderTarget(RenderTarget target) {
	switch (target) {
	case(RT_TOPLEFT):
		return targetTopLeft_;
	case(RT_TOPRIGHT):
		return targetTopRight_;
	case(RT_BOTTOM):
		return targetBottom_;
	default:
		Error::throwError("Attempted to get non-existant RenderTarget");
		return nullptr; // to appease the warnings
		//exec stop
	}
}

RenderManager::~RenderManager()
{
	C3D_Fini();
	gfxExit();
}

RenderManager RenderManager::instance_;

RenderManager& RenderManager::instance() {
	return instance_;
}

