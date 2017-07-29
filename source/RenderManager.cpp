#include "Globals.h"


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
	//linearFree(vshader_dvlb); //Unsure if I'm meant to free it or not

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

void RenderManager::beginFrame() {
	if (!gfxIs3D()) {
		gfxSet3D(true);
	}
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
}

void RenderManager::endFrame() {
	C3D_FrameEnd(0);
	//gspWaitForVBlank();
}


void RenderManager::renderModel(Model model, Transform transform, RenderTarget target) {
	if (model.getNumVertices() < 1)
		return;

	// Uniform locations
	int uLoc_projection, uLoc_modelView;
	int uLoc_lightVec, uLoc_lightHalfVec, uLoc_lightClr, uLoc_material;
	C3D_Mtx projection;

	// Create a material for the model
	C3D_Mtx material =
	{
		{
			{ { 0.0f, 0.2f, 0.2f, 0.2f } }, // Ambient
			{ { 0.0f, 0.4f, 0.4f, 0.4f } }, // Diffuse
			{ { 0.0f, 0.8f, 0.8f, 0.8f } }, // Specular
			{ { 1.0f, 0.0f, 0.0f, 0.0f } }, // Emission
		}
	};

	//Bind the default shaderprogram
	C3D_BindProgram(&defaultShaderProgram_);

	// Get uniform locations
	uLoc_projection = shaderInstanceGetUniformLocation(defaultShaderProgram_.vertexShader, "projection");
	uLoc_modelView = shaderInstanceGetUniformLocation(defaultShaderProgram_.vertexShader, "modelView");
	uLoc_lightVec = shaderInstanceGetUniformLocation(defaultShaderProgram_.vertexShader, "lightVec");
	uLoc_lightHalfVec = shaderInstanceGetUniformLocation(defaultShaderProgram_.vertexShader, "lightHalfVec");
	uLoc_lightClr = shaderInstanceGetUniformLocation(defaultShaderProgram_.vertexShader, "lightClr");
	uLoc_material = shaderInstanceGetUniformLocation(defaultShaderProgram_.vertexShader, "material");
	
	// Configure attributes for use with the vertex shader
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=normal

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, model.getVBO(), sizeof(Model::vertex), 3, 0x210);

	C3D_TexBind(0, model.getTexture());
	
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

	float scale = transform.getScale();
	vec3f eulerAngles = transform.getEulerRotation(),
		position = transform.getPos();

	C3D_Mtx modelView;
	Mtx_Identity(&modelView);
	Mtx_Scale(&modelView, scale, scale, scale);
	Mtx_Translate(&modelView, position.x, position.y, position.z, true);
	Mtx_RotateX(&modelView, eulerAngles.x, true);
	Mtx_RotateY(&modelView, eulerAngles.y, true);
	Mtx_RotateZ(&modelView, eulerAngles.z, true);
	
	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView, &modelView);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_material, &material);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightClr, 1.0f, 1.0f, 1.0f, 1.0f);
	
	if (target == RT_TOPLEFT || target == RT_TOP || target == RT_ALL) {
		Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(50.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, separationMultiplier_ * -osGet3DSliderState(), 2.0f, false);
		C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
		C3D_FrameDrawOn(targetTopLeft_);
		C3D_DrawArrays(GPU_TRIANGLES, 0, model.getNumVertices());
	}

	if (target == RT_TOPRIGHT || target == RT_TOP || target == RT_ALL) {
		Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(50.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, separationMultiplier_ * osGet3DSliderState(), 2.0f, false);
		C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
		C3D_FrameDrawOn(targetTopRight_);
		C3D_DrawArrays(GPU_TRIANGLES, 0, model.getNumVertices());
	}

	if (target == RT_BOTTOM || target == RT_ALL) {
		Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(50.0f), C3D_AspectRatioBot, 0.01f, 1000.0f, false);
		C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
		C3D_FrameDrawOn(targetBottom_);
		C3D_DrawArrays(GPU_TRIANGLES, 0, model.getNumVertices());
	}

	
	
	linearFree(attrInfo);
	linearFree(bufInfo);
	linearFree(env);
}
