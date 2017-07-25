#include "TextNode.h"


TextNode::TextNode(SceneNode* parent, TextNodeOpts opts) :SceneNode(parent)
{
	util::debug_print(__PRETTY_FUNCTION__);
	options = opts;

	std::streamsize size;
	std::vector<unsigned char> buffer, image;

	std::ifstream file(opts.characterset);
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	//read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else buffer.clear();

	unsigned long w, h;
	int error = decodePNG(image, w, h, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size());
	if (error != 0) {
		char info[30];
		sprintf(info, "Image (%s) load failed! Error:%i", opts.characterset.c_str(), error);
		util::throw_error(info);
	}
	util::convertBitmapToTiledImg(&image[0], w, h, &tiledTexture);


}


void TextNode::update() {
	
	// Create uniform location varables
	int uLoc_projection, uLoc_modelView;
	int uLoc_lightVec, uLoc_lightHalfVec, uLoc_lightClr, uLoc_material;
	C3D_Mtx projectionl, projectionr, projectionb;

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
	C3D_BindProgram(&util::DEFAULT_SHADERPROGRAM);

	// Get the locations of the uniforms
	uLoc_projection = shaderInstanceGetUniformLocation(util::DEFAULT_SHADERPROGRAM.vertexShader, "projection");
	uLoc_modelView = shaderInstanceGetUniformLocation(util::DEFAULT_SHADERPROGRAM.vertexShader, "modelView");
	uLoc_lightVec = shaderInstanceGetUniformLocation(util::DEFAULT_SHADERPROGRAM.vertexShader, "lightVec");
	uLoc_lightHalfVec = shaderInstanceGetUniformLocation(util::DEFAULT_SHADERPROGRAM.vertexShader, "lightHalfVec");
	uLoc_lightClr = shaderInstanceGetUniformLocation(util::DEFAULT_SHADERPROGRAM.vertexShader, "lightClr");
	uLoc_material = shaderInstanceGetUniformLocation(util::DEFAULT_SHADERPROGRAM.vertexShader, "material");

	// Configure attributes for use with the vertex shader
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=normal

	// Create the VBO (vertex buffer object)
	void* vbo_data = linearAlloc(sizeof(vertex_list));
	memcpy(vbo_data, vertex_list, sizeof(vertex_list));

	// Configure buffers
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vbo_data, sizeof(util::vertex), 3, 0x210);

	// Bind our texture
	C3D_TexBind(0, &tiledTexture);

	// Configure the first fragment shading substage to blend the texture color with
	// the vertex color (calculated by the vertex shader using a lighting algorithm)
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

	// Compute the projection matrix
	Mtx_PerspStereoTilt(&projectionl, C3D_AngleFromDegrees(80.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, util::get3DOffset(), 2.0f,false);
	Mtx_PerspStereoTilt(&projectionr, C3D_AngleFromDegrees(80.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, -util::get3DOffset(), 2.0f, false);
	Mtx_PerspTilt(&projectionb, C3D_AngleFromDegrees(80.0f), C3D_AspectRatioBot, 0.01f, 1000.0f, false);


	// Create a modelview matrix ( 2 units into the screen)
	C3D_Mtx modelView;
	Mtx_Identity(&modelView);
	Mtx_Translate(&modelView, 0.0, 0.0, -2.0, true);

	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projectionl);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView, &modelView);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_material, &material);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightClr, 1.0f, 1.0f, 1.0f, 1.0f);

	//Specify which target we are drawing to
	C3D_FrameDrawOn(util::RT_TOPLEFT);

	// Draw the VBO
	C3D_DrawArrays(GPU_TRIANGLES, 0, sizeof(vertex_list) / sizeof(util::vertex));

	Mtx_Identity(&modelView);
	Mtx_Translate(&modelView, 0.0, 0.0, -2.0, true);

	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projectionr);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView, &modelView);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_material, &material);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightClr, 1.0f, 1.0f, 1.0f, 1.0f);

	//Specify which target we are drawing to
	C3D_FrameDrawOn(util::RT_TOPRIGHT);

	// Draw the VBO
	C3D_DrawArrays(GPU_TRIANGLES, 0, sizeof(vertex_list) / sizeof(util::vertex));

	Mtx_Identity(&modelView);
	Mtx_Translate(&modelView, 0.0, 0.0, -2.0, true);

	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projectionb);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView, &modelView);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_material, &material);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightClr, 1.0f, 1.0f, 1.0f, 1.0f);

	//Specify which target we are drawing to
	C3D_FrameDrawOn(util::RT_BOTTOM);

	// Draw the VBO
	C3D_DrawArrays(GPU_TRIANGLES, 0, sizeof(vertex_list) / sizeof(util::vertex));

	// Free our pointers
	linearFree(env);
	linearFree(vbo_data);
	linearFree(attrInfo);
	
}

NodeType::NodeType TextNode::getNodeType(){
	return TextNode::type;
}

TextNode::~TextNode()
{
	util::debug_print("Closing " + NodeType::toString(getNodeType()));
}
