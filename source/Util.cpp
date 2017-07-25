#include "Util.h"

#define TK_OBJFILE_IMPLEMENTATION
#include "Lib/tk_objfile.h"

namespace util {

	shaderProgram_s DEFAULT_SHADERPROGRAM;
	C3D_RenderTarget* RT_TOPLEFT, *RT_TOPRIGHT, *RT_BOTTOM;

	void loadShaders() {
		DVLB_s* vshader_dvlb;
		vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
		shaderProgramInit(&DEFAULT_SHADERPROGRAM);
		shaderProgramSetVsh(&DEFAULT_SHADERPROGRAM, &vshader_dvlb->DVLE[0]);
		// add more shaders under here

	}

	float get3DOffset() {
		return THREEDEE_MULTIPLIER * osGet3DSliderState();
	}

	void throw_error(std::string text) {
		debug_print(text);

		errorConf err;
		errorInit(&err, errorType::ERROR_TEXT_WORD_WRAP, CFG_Language::CFG_LANGUAGE_EN);
		errorText(&err, text.c_str());
		errorDisp(&err);
		svcExitProcess();
		stop(); // for good measure
	}

	void debug_print(std::string text) {
#ifdef _DEBUG_

		FILE* logfile = fopen("_Application.log", "ab+");
		fseek(logfile, 0, SEEK_END);
		fwrite(text.c_str(), sizeof(char), text.size(), logfile);
		fwrite(std::string("\n").c_str(), sizeof(char), 1, logfile);
		fclose(logfile);

		consoleDebugInit(debugDevice_SVC);
		std::cerr << text << std::endl;
#endif
	}

	unsigned char* loadImgToBitmap(std::string filepath, unsigned long* width, unsigned long* height) {

		// Create things
		std::streamsize size = 0;
		std::vector<unsigned char> data_buffer, image_processed;

		// Open filestream
		std::ifstream file(filepath);
		if (!file) {
			throw_error("Image file (" + filepath + ") does not exist");
		}


		if (file.seekg(0, std::ios::end).good()) {
			size = file.tellg();
		}
		if (file.seekg(0, std::ios::beg).good())
		{
			size -= file.tellg();
		}

		//read contents of the file into the vector
		if (size > 0)
		{
			data_buffer.resize((size_t)size);
			file.read((char*)(&data_buffer[0]), size);
		}
		else {
			data_buffer.clear();
			throw_error("Image file (" + filepath + ") contained no data");
		}

		// Attempt to decode the png
		unsigned long w, h;
		int error = decodePNG(image_processed, w, h, data_buffer.empty() ? 0 : &data_buffer[0], (unsigned long)data_buffer.size());
		if (error != 0) {
			throw_error("Image file (" + filepath + ") cannot be decoded.");
		}

		// Allocate space for image in ram
		unsigned char* image = (unsigned char*)linearAlloc(sizeof(unsigned char) * w * h * 4);

		// copy into params
		memcpy(image, image_processed.data(), sizeof(unsigned char) * w * h * 4);
		*width = w;
		*height = h;

		return image;
	}

	// Converts a bitmap texture to a C3D_Tex and performs the tiling operation on the GPU
	void convertBitmapToTiledImg(unsigned char* inImage, unsigned long inImgWidth, unsigned long inImgHeight, C3D_Tex* outImage) {

		// Create storage for swapping and tiling
		u8 *gpusrc = (u8*)linearAlloc(inImgWidth*inImgHeight * 4);

		// GX_DisplayTransfer needs input buffer in linear RAM
		u8* src = inImage; u8 *dst = gpusrc;

		// lodepng outputs big endian rgba so we need to convert
		for (unsigned int i = 0; i<inImgWidth*inImgHeight; i++) {
			int r = *src++;
			int g = *src++;
			int b = *src++;
			int a = *src++;

			*dst++ = a;
			*dst++ = b;
			*dst++ = g;
			*dst++ = r;
		}

		// ensure data is in physical ram
		GSPGPU_FlushDataCache(gpusrc, inImgWidth*inImgHeight * 4);

		// Load the texture and bind it to the first texture unit
		C3D_TexInit(outImage, inImgWidth, inImgHeight, GPU_RGBA8);

		// Convert image to 3DS tiled texture format
		C3D_SafeDisplayTransfer((u32*)gpusrc, GX_BUFFER_DIM(inImgWidth, inImgHeight), (u32*)outImage->data, GX_BUFFER_DIM(inImgWidth, inImgHeight), TEXTURE_TRANSFER_FLAGS);
		gspWaitForPPF();

		linearFree(gpusrc);
	}


	void initC3D() {
		gfxInitDefault();
		C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
		
		RT_TOPLEFT = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
		C3D_RenderTargetSetClear(RT_TOPLEFT, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
		C3D_RenderTargetSetOutput(RT_TOPLEFT, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

		RT_TOPRIGHT = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
		C3D_RenderTargetSetClear(RT_TOPRIGHT, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
		C3D_RenderTargetSetOutput(RT_TOPRIGHT, GFX_TOP, GFX_RIGHT, DISPLAY_TRANSFER_FLAGS);

		RT_BOTTOM = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
		C3D_RenderTargetSetClear(RT_BOTTOM, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
		C3D_RenderTargetSetOutput(RT_BOTTOM, GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
	}
	
	static unsigned long long _objNumVerts = 0;
	vertex* loadOBJ(std::string filepath, std::size_t* numVerts) {

		//Create a pointer we will return with our vertex buffer
		vertex* vertexArray;

		// Create obj loader thing
		TK_ObjDelegate objDelegate = {};
		
		//Reset our counter for the new obj
		_objNumVerts = 0;

		// Set func to run for each triangle
		objDelegate.triangle = [](TK_TriangleVert a, TK_TriangleVert b, TK_TriangleVert c, void* data) {
		// Do stuff with triangle (ie add to vertexArray)
			vertex v1,v2,v3;
			
			//Copy over v1
			v1.position[0] = a.pos[0];
			v1.position[1] = a.pos[1];
			v1.position[2] = a.pos[2];
			v1.normal[0] = a.nrm[0];
			v1.normal[1] = a.nrm[1];
			v1.normal[2] = a.nrm[2];
			v1.texcoord[0] = a.st[0];
			v1.texcoord[1] = a.st[1];

			//Copy over v2
			v2.position[0] = b.pos[0];
			v2.position[1] = b.pos[1];
			v2.position[2] = b.pos[2];
			v2.normal[0] = b.nrm[0];
			v2.normal[1] = b.nrm[1];
			v2.normal[2] = b.nrm[2];
			v2.texcoord[0] = b.st[0];
			v2.texcoord[1] = b.st[1];

			//Copy over v3
			v3.position[0] = c.pos[0];
			v3.position[1] = c.pos[1];
			v3.position[2] = c.pos[2];
			v3.normal[0] = c.nrm[0];
			v3.normal[1] = c.nrm[1];
			v3.normal[2] = c.nrm[2];
			v3.texcoord[0] = c.st[0];
			v3.texcoord[1] = c.st[1];

			// Get pointer to our existing vertices
			vertex* vertices = (vertex*)data;
			// Copy our 3 new vertices in
			memcpy(&vertices[_objNumVerts], &v1, sizeof(vertex));
			memcpy(&vertices[_objNumVerts+1], &v2, sizeof(vertex));
			memcpy(&vertices[_objNumVerts+2], &v3, sizeof(vertex));

			_objNumVerts += 3;
		};

		std::streamsize size;
		std::vector<char> buffer;

		std::ifstream file(filepath);
		if (!file) {
			throw_error("OBJ file (" + filepath + ") does not exist");
		}


		if (file.seekg(0, std::ios::end).good()) size = file.tellg();
		if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

		//read contents of the file into the vector
		if (size > 0)
		{
			buffer.resize((size_t)size);
			file.read((char*)(&buffer[0]), size);
		}
		else {
			buffer.clear();
			throw_error("OBJ file (" + filepath + ") contained no data");
		}

		// Prepass to determine memory requirements
		objDelegate.scratchMemSize = 0;
		TK_ParseObj(&buffer[0], size, &objDelegate);

		//Give the scratch some memory
		objDelegate.scratchMem = linearAlloc(objDelegate.scratchMemSize);

		// Allocate some memory for our vertex buffer
		vertexArray = (vertex*)linearAlloc(sizeof(vertex) * objDelegate.numVerts);

		// Pass our buffer into to the triangle function
		objDelegate.userData = (void*)vertexArray;

		// Parse again with memory. This will call material() and
		// triangle() callbacks
		TK_ParseObj(&buffer[0], size, &objDelegate);

		// Output the number of vertices
		*numVerts = objDelegate.numVerts;

		// Free the scratch memory
		linearFree(objDelegate.scratchMem);
		
		util::debug_print("Loaded " + filepath + " successfully!");
		return vertexArray;
	}
}