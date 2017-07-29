#include "Loader.h"

Loader::Loader(){

}

Loader::~Loader() {

}

int Loader::texConvertFlags =
(GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

Bitmap Loader::loadToBitmap(std::string filepath) {
	Bitmap picture;
	std::string fullPath = "romfs:" + filepath;
	std::vector<unsigned char> image; //the raw pixels

	// decode
	if (lodepng::decode(image, picture.width, picture.height, fullPath))
		Error::throwError("Cannot load or decode PNG file " + fullPath);
	picture.datalen = picture.width * picture.height;
	picture.pixels = (unsigned char*)linearAlloc(sizeof(unsigned char) * picture.datalen);
	
	//copy into struct
	memcpy(picture.pixels, image.data(), sizeof(unsigned char) * picture.datalen);
	
	return picture;
}


void Loader::bitmapToTiled(Bitmap inImage, C3D_Tex* outImage) {
	bitmapToTiled(inImage.pixels, inImage.width, inImage.height, outImage);
}
void Loader::bitmapToTiled(unsigned char* inImage, unsigned long inImgWidth, unsigned long inImgHeight, C3D_Tex* outImage) {

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
	C3D_SafeDisplayTransfer((u32*)gpusrc, GX_BUFFER_DIM(inImgWidth, inImgHeight), (u32*)outImage->data, GX_BUFFER_DIM(inImgWidth, inImgHeight), texConvertFlags);
	gspWaitForPPF();

	linearFree(gpusrc);
}