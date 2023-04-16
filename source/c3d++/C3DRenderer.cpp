

#include <c3d++/C3DRenderer.h>

namespace C3DRenderer {
namespace local {
    std::unique_ptr<C3DShader> defaultShader_;

    C3D_RenderTarget *targetTopLeft_, *targetTopRight_, *targetBottom_;

    int clearColor_ = 0x0000FFFF;

    int renderFlags_ = (GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

    const float separationMultiplier_ = 0.3;

    C3DRenderTarget currentTarget = C3DRenderTarget::TOP;

    C3DTransform cameraTransform_;

    bool shadeless_ = false;

    std::size_t triCount_ = 0;

    mat<float, 4, 4> viewMatrix_;
}

void setCameraTransform(C3DTransform cameraTransform)
{
    local::cameraTransform_ = std::move(cameraTransform);
}

C3DTransform const& getCameraTransform()
{
    return local::cameraTransform_;
}

void initRenderer()
{
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

    local::targetTopLeft_ = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    C3D_RenderTargetClear(local::targetTopLeft_, C3D_CLEAR_ALL, local::clearColor_, 0);
    C3D_RenderTargetSetOutput(local::targetTopLeft_, GFX_TOP, GFX_LEFT, local::renderFlags_);

    local::targetTopRight_ = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    C3D_RenderTargetClear(local::targetTopRight_, C3D_CLEAR_ALL, local::clearColor_, 0);
    C3D_RenderTargetSetOutput(local::targetTopRight_, GFX_TOP, GFX_RIGHT, local::renderFlags_);

    local::targetBottom_ = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    C3D_RenderTargetClear(local::targetBottom_, C3D_CLEAR_ALL, local::clearColor_, 0);
    C3D_RenderTargetSetOutput(local::targetBottom_, GFX_BOTTOM, GFX_LEFT, local::renderFlags_);

    local::defaultShader_ = std::make_unique<C3DShader>(v_default_shbin, v_default_shbin_size);
}

void closeRenderer()
{
    C3D_RenderTargetDelete(local::targetTopLeft_);
    C3D_RenderTargetDelete(local::targetTopRight_);
    C3D_RenderTargetDelete(local::targetBottom_);
    C3D_Fini();
    gfxExit();
}

void beginFrame()
{
    std::cout << "Rendered " << local::triCount_ << " tris" << std::endl;
    local::triCount_ = 0;
    if (!gfxIs3D()) {
        gfxSet3D(true);
    }
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    /* VIEW MATRIX */
    vec<float, 3> cYPR = local::cameraTransform_.getYPR();
    vec<float, 3> cPos = local::cameraTransform_.getPos();

    mat<float, 4, 4> cMatYaw = rotation_matrix(rotation_quat(vec<float, 3> { 0, 1, 0 }, -cYPR.x));
    mat<float, 4, 4> cMatPitch = rotation_matrix(rotation_quat(vec<float, 3> { 1, 0, 0 }, -cYPR.y));
    mat<float, 4, 4> cMatRoll = rotation_matrix(rotation_quat(vec<float, 3> { 0, 0, 1 }, -cYPR.z));
    mat<float, 4, 4> cMatPos = translation_matrix(-cPos);

    mat<float, 4, 4> cMatYPR = mul(cMatRoll, mul(cMatPitch, cMatYaw));

    local::viewMatrix_ = mul(cMatYPR, cMatPos);
}

void endFrame()
{
    C3D_FrameEnd(0);
}

void setTarget(C3DRenderTarget target)
{
    local::currentTarget = target;
}

void drawNextShadeless()
{
    local::shadeless_ = true;
}

void draw(C3DModel const& model, C3DTransform const& transform)
{
    auto shader = model.getShader();
    draw(model.getMesh(), model.getTexture(), shader.get() == nullptr ? *local::defaultShader_.get() : *shader.get(), transform);
}

void draw(C3DMesh const& mesh, C3DTexture const& texture, C3DShader& shader, C3DTransform const& transform)
{
    if (mesh.getVertices().size() < 1)
        return;

    // Uniform locations
    int uLoc_projection, uLoc_modelView;
    int uLoc_lightVec, uLoc_lightHalfVec, uLoc_lightClr, uLoc_material;
    C3D_Mtx projection;

    // Create a material for the model
    C3D_Mtx material;

    if (local::shadeless_) {
        material = {
            {
                { { 0.0f, 0.2f, 0.2f, 0.2f } }, // Ambient
                { { 0.0f, 0.4f, 0.4f, 0.4f } }, // Diffuse
                { { 0.0f, 0.8f, 0.8f, 0.8f } }, // Specular
                { { 1.0f, 1.0f, 1.0f, 1.0f } }, // Emission
            }
        };
        local::shadeless_ = false;
    } else {
        material = {
            {
                { { 0.0f, 0.2f, 0.2f, 0.2f } }, // Ambient
                { { 0.0f, 0.4f, 0.4f, 0.4f } }, // Diffuse
                { { 0.0f, 0.8f, 0.8f, 0.8f } }, // Specular
                { { 1.0f, 0.0f, 0.0f, 0.0f } }, // Emission
            }
        };
    }

    shaderProgram_s& shaderProgram = shader.getShaderProgram();

    if (!shader.valid()) {
        Error::throwError("Shader is not valid");
    }
    C3D_BindProgram(&shaderProgram);

    // Get uniform locations
    uLoc_projection = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "projection");
    uLoc_modelView = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "modelView");
    uLoc_lightVec = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "lightVec");
    uLoc_lightHalfVec = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "lightHalfVec");
    uLoc_lightClr = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "lightClr");
    uLoc_material = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "material");

    // Configure attributes for use with the vertex shader
    C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
    AttrInfo_Init(attrInfo);
    AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
    AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord
    AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=normal

    C3D_BufInfo* bufInfo = C3D_GetBufInfo();
    BufInfo_Init(bufInfo);
    BufInfo_Add(bufInfo, mesh.getVBO().data(), sizeof(Vertex::value_type) * Vertex::stride, 3, 0x210);

    C3D_CullFace(GPU_CULLMODE::GPU_CULL_FRONT_CCW);
    C3D_TexBind(0, const_cast<C3D_Tex*>(&texture.getCTexture()));

    C3D_TexEnv* env = C3D_GetTexEnv(0);
    C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, GPU_TEVSRC::GPU_PRIMARY_COLOR);
    C3D_TexEnvOpRgb(env, GPU_TEVOP_RGB::GPU_TEVOP_RGB_SRC_COLOR, GPU_TEVOP_RGB::GPU_TEVOP_RGB_SRC_COLOR, GPU_TEVOP_RGB::GPU_TEVOP_RGB_SRC_COLOR);
    C3D_TexEnvOpAlpha(env, GPU_TEVOP_A::GPU_TEVOP_A_SRC_ALPHA, GPU_TEVOP_A::GPU_TEVOP_A_SRC_ALPHA, GPU_TEVOP_A::GPU_TEVOP_A_SRC_ALPHA);
    C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

    /* MODEL MATRIX */
    vec<float, 3> mYPR = transform.getYPR();
    vec<float, 3> mPos = transform.getPos();
    float mScale = transform.getScale();

    mat<float, 4, 4> mMatYaw = rotation_matrix(rotation_quat(vec<float, 3> { 0, 1, 0 }, mYPR.x));
    mat<float, 4, 4> mMatPitch = rotation_matrix(rotation_quat(vec<float, 3> { 1, 0, 0 }, mYPR.y));
    mat<float, 4, 4> mMatRoll = rotation_matrix(rotation_quat(vec<float, 3> { 0, 0, 1 }, mYPR.z));
    mat<float, 4, 4> mMatPos = translation_matrix(mPos);
    mat<float, 4, 4> mMatScale = scaling_matrix(vec<float, 3> { mScale, mScale, mScale });

    mat<float, 4, 4> mMatYPR = mul(mMatRoll, mul(mMatPitch, mMatYaw));

    mat<float, 4, 4> modelMat = mul(mul(mMatPos, mMatScale), mMatYPR);

    mat<float, 4, 4> modelViewMat = mul(local::viewMatrix_, modelMat);

    C3D_Mtx C3D_ModelViewMat;
    for (int i = 0; i < 4; i++) {
        C3D_ModelViewMat.m[i * 4 + 0] = modelViewMat.row(i).w;
        C3D_ModelViewMat.m[i * 4 + 1] = modelViewMat.row(i).x;
        C3D_ModelViewMat.m[i * 4 + 2] = modelViewMat.row(i).y;
        C3D_ModelViewMat.m[i * 4 + 3] = modelViewMat.row(i).z;
    }

    /* */

    // Update the uniforms
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView, &C3D_ModelViewMat);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_material, &material);
    C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightVec, -1.0f, -1.0f, -1.0f, 0.0f);
    C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 0.0f, 0.0f, -1.0f, 0.0f);
    C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightClr, 1.0f, 1.0f, 1.0f, 1.0f);

    C3D_AlphaTest(true, GPU_GREATER, 0);

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

    local::triCount_ += mesh.getVertices().size();

    linearFree(attrInfo);
    linearFree(bufInfo);
    linearFree(env);
}

void nextLayer()
{
    C3D_FrameSplit(0);
    C3D_FrameBufClear(&(local::targetTopLeft_->frameBuf), C3D_CLEAR_DEPTH, 0, 0);
    C3D_FrameBufClear(&(local::targetTopRight_->frameBuf), C3D_CLEAR_DEPTH, 0, 0);
    C3D_FrameBufClear(&(local::targetBottom_->frameBuf), C3D_CLEAR_DEPTH, 0, 0);
}

// Frustum createFrustumFromCamera(float aspect, float fovY, float zNear, float zFar)
// {
//     Frustum frustum;
//     const float halfVSide = zFar * tanf(fovY * .5f);
//     const float halfHSide = halfVSide * aspect;
//     const auto forward = local::cameraTransform_.getForward();
//     const auto right = local::cameraTransform_.getRight();
//     const auto up = local::cameraTransform_.getUp();
//     const auto frontMultFar = zFar * forward;
//     auto pos = local::cameraTransform_.getPos();

//     return frustum;
// }
};