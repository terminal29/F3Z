#include <c3d++/C3DRenderer.h>

namespace C3DRenderer {
	namespace local {
		shaderProgram_s defaultShaderProgram_;
		C3D_RenderTarget* targetTopLeft_, *targetTopRight_, *targetBottom_;

		int clearColor_ = 0x0000FFFF;

		int renderFlags_ =
			(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
				GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
				GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

		const float separationMultiplier_ = 0.3;

		C3DRenderTarget currentTarget = C3DRenderTarget::TOP;
	}

	void initRenderer() {
		gfxInitDefault();
		C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

		local::targetTopLeft_ = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
		C3D_RenderTargetSetClear(local::targetTopLeft_, C3D_CLEAR_ALL, local::clearColor_, 0);
		C3D_RenderTargetSetOutput(local::targetTopLeft_, GFX_TOP, GFX_LEFT, local::renderFlags_);

		local::targetTopRight_ = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
		C3D_RenderTargetSetClear(local::targetTopRight_, C3D_CLEAR_ALL, local::clearColor_, 0);
		C3D_RenderTargetSetOutput(local::targetTopRight_, GFX_TOP, GFX_RIGHT, local::renderFlags_);

		local::targetBottom_ = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
		C3D_RenderTargetSetClear(local::targetBottom_, C3D_CLEAR_ALL, local::clearColor_, 0);
		C3D_RenderTargetSetOutput(local::targetBottom_, GFX_BOTTOM, GFX_LEFT, local::renderFlags_);

		// Load shader (s)
		DVLB_s* vshader_dvlb;
		vshader_dvlb = DVLB_ParseFile((u32*)v_default_shbin, v_default_shbin_size);
		shaderProgramInit(&local::defaultShaderProgram_);
		shaderProgramSetVsh(&local::defaultShaderProgram_, &vshader_dvlb->DVLE[0]);

	}

	void closeRenderer() {
		C3D_RenderTargetDelete(local::targetTopLeft_);
		C3D_RenderTargetDelete(local::targetTopRight_);
		C3D_RenderTargetDelete(local::targetBottom_);
		C3D_Fini();
		gfxExit();
	}

	void beginFrame() {
		if (!gfxIs3D()) {
			gfxSet3D(true);
		}
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	}

	void endFrame() {
		C3D_FrameEnd(0);
	}

	void setTarget(C3DRenderTarget target) {
		local::currentTarget = target;
	}

	void draw(C3DModel& model, C3DTransform& transform) {
		draw(model.getMesh(), model.getTexture(), transform);
	}

	void draw(C3DMesh& mesh, C3DTexture& texture, C3DTransform& transform) {
		if (mesh.getVertices().size() < 1)
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
		C3D_BindProgram(&local::defaultShaderProgram_);

		// Get uniform locations
		uLoc_projection = shaderInstanceGetUniformLocation(local::defaultShaderProgram_.vertexShader, "projection");
		uLoc_modelView = shaderInstanceGetUniformLocation(local::defaultShaderProgram_.vertexShader, "modelView");
		uLoc_lightVec = shaderInstanceGetUniformLocation(local::defaultShaderProgram_.vertexShader, "lightVec");
		uLoc_lightHalfVec = shaderInstanceGetUniformLocation(local::defaultShaderProgram_.vertexShader, "lightHalfVec");
		uLoc_lightClr = shaderInstanceGetUniformLocation(local::defaultShaderProgram_.vertexShader, "lightClr");
		uLoc_material = shaderInstanceGetUniformLocation(local::defaultShaderProgram_.vertexShader, "material");

		// Configure attributes for use with the vertex shader
		C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
		AttrInfo_Init(attrInfo);
		AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
		AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord
		AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=normal

		C3D_BufInfo* bufInfo = C3D_GetBufInfo();
		BufInfo_Init(bufInfo);
		BufInfo_Add(bufInfo, mesh.getVBO(), sizeof(Vertex), 3, 0x210);


		C3D_TexBind(0, texture.getCTexture());

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
		C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightVec, -1.0f, -1.0f, -1.0f, 0.0f);
		C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 0.0f, 0.0f, -1.0f, 0.0f);
		C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightClr, 1.0f, 1.0f, 1.0f, 1.0f);

		if (local::currentTarget == C3DRenderTarget::LEFT || local::currentTarget == C3DRenderTarget::TOP) {
			Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(50.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, local::separationMultiplier_ * -osGet3DSliderState(), 2.0f, false);
			C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
			C3D_FrameDrawOn(local::targetTopLeft_);
			C3D_DrawArrays(GPU_TRIANGLES, 0, mesh.getVertices().size());
		}

		if (local::currentTarget == C3DRenderTarget::RIGHT || local::currentTarget == C3DRenderTarget::TOP) {
			Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(50.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, local::separationMultiplier_ * osGet3DSliderState(), 2.0f, false);
			C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
			C3D_FrameDrawOn(local::targetTopRight_);
			C3D_DrawArrays(GPU_TRIANGLES, 0, mesh.getVertices().size());
		}

		if (local::currentTarget == C3DRenderTarget::BOTTOM) {
			Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(50.0f), C3D_AspectRatioBot, 0.01f, 1000.0f, false);
			C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
			C3D_FrameDrawOn(local::targetBottom_);
			C3D_DrawArrays(GPU_TRIANGLES, 0, mesh.getVertices().size());
		}

		linearFree(attrInfo);
		linearFree(bufInfo);
		linearFree(env);
	}

	void nextLayer() {
		C3D_FrameSplit(0);
		C3D_FrameBufClear(&(local::targetTopLeft_->frameBuf), C3D_CLEAR_DEPTH, 0, 0);
		C3D_FrameBufClear(&(local::targetTopRight_->frameBuf), C3D_CLEAR_DEPTH, 0, 0);
		C3D_FrameBufClear(&(local::targetBottom_->frameBuf), C3D_CLEAR_DEPTH, 0, 0);
	}
};