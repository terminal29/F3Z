#include "Util.h"

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

		errorConf err;
		errorInit(&err, errorType::ERROR_TEXT_WORD_WRAP, CFG_Language::CFG_LANGUAGE_EN);
		errorText(&err, text.c_str());
		errorDisp(&err);
		svcExitProcess();
		stop(); // for good measure
	}

	void debug_print(std::string text) {
#ifdef _DEBUG_

	/*	FILE* logfile = fopen("_Application.log", "ab+");
		fseek(logfile, 0, SEEK_END);
		fwrite(text.c_str(), sizeof(char), text.size(), logfile);
		fwrite(std::string("\n").c_str(), sizeof(char), 1, logfile);
		fclose(logfile);*/

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
	/*
	vertex* loadOBJ(std::string filepath) {
		vertex* vertexArray;
		// Set processTriangle func to run for each tri
		TK_ObjDelegate objDelegate = {};
		objDelegate.triangle = [](TK_TriangleVert a, TK_TriangleVert b, TK_TriangleVert c, void* data) {
		// Do stuff with triangle (ie add to vertexArray)
			std::stringstream info;
			info << a.pos[0] << ":" << a.pos[1] << ":" << a.pos[2];
			util::debug_print(info.str());
			stop();
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

		objDelegate.scratchMem = malloc(objDelegate.scratchMemSize);

		// Parse again with memory. This will call material() and
		// triangle() callbacks
		TK_ParseObj(&buffer[0], size, &objDelegate);
	}*/
}