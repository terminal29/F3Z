#pragma once
#define _DEBUG_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

#include <memory.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <sstream>

#include <citro3d.h>
#include <3ds.h>
#include "Lib/picopng.h"



/* compiled shaders */
#include "vshader_shbin.h"

#define THREEDEE_MULTIPLIER 0.4

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))


#define TEXTURE_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

namespace util {
	
	// Stuff for rendering
	extern shaderProgram_s DEFAULT_SHADERPROGRAM;
	extern C3D_RenderTarget* RT_TOPLEFT, *RT_TOPRIGHT, *RT_BOTTOM;

	// Vertex struct
	typedef struct { float position[3]; float texcoord[2]; float normal[3]; } vertex;
	
	/* Loads shaders into their respective variables */
	extern void loadShaders();

	/* Gets the 3d offset based on the slider and some multiplier */
	extern float get3DOffset();

	/* Initiaised Citro3D */
	extern void initC3D();

	/* Loads a PNG to a RGBARGBARGBA... bitmap in memory*/
	extern unsigned char* loadImgToBitmap(std::string filepath, unsigned long* width, unsigned long* height);

	/* Converts a bitmap RGBARGBARGBA... to a C3D_Tex in tiled format */
	extern void convertBitmapToTiledImg(unsigned char* inImage, unsigned long inImgWidth, unsigned long inImgHeight, C3D_Tex* outImage);

	// Prints a string to the log file or to the debug output
	extern void debug_print(std::string text);

	// Quick n dirty err function which displays an error then kills the process
	extern void throw_error(std::string text);

	// Debug func to effectively halt execution.
	inline void stop() {
		while (true) {
			svcSleepThread(1000000);
		}
	}

	// Loads an OBJ file from the filepath and returns a vertex array
	extern vertex* loadOBJ(std::string filepath, std::size_t* numVerts);

}