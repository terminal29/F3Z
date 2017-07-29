#pragma once
#include "Globals.h"
#include <string>
#include "library/lodepng.h"
#include <citro3d.h>

struct Bitmap {
	unsigned char* pixels;
	unsigned int width = 0, height = 0, datalen = 0;
};

class Loader {
public:
	static Bitmap loadToBitmap(std::string filepath);
	static void bitmapToTiled(unsigned char* inImage, unsigned long inImgWidth, unsigned long inImgHeight, C3D_Tex* outImage);
	static void bitmapToTiled(Bitmap inImage, C3D_Tex* outImage);
private:
	Loader();
	~Loader();

	static int texConvertFlags;

};