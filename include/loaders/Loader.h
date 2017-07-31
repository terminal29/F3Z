#pragma once
#include "Globals.h"
#include <string>
#include "library/lodepng.h"
#include <citro3d.h>
#include "entity/component/Model.h"
#include "library/json/json.h"
#include "RenderManager.h"
#include "Types.h"


struct StaticTile;

class Loader {
public:

	/* Loads a file from the romfs to a bitmap */
	static Bitmap loadToBitmap(std::string filepath);
	
	/* Converts a raw uchar buffer to a tiled C3D_Tex */
	static void bitmapToTiled(unsigned char* inImage, unsigned long inImgWidth, unsigned long inImgHeight, C3D_Tex* outImage);
	
	/* Converts a Bitmap to a tiled C3D_Tex */
	static void bitmapToTiled(Bitmap inImage, C3D_Tex* outImage);

	/* Loads an obj file from the romfs to a Model file */
	static Model loadOBJ(std::string filepath);

	/* Loads all of the static tiles required for a specific island */
	static std::vector<StaticTile> loadStaticTiles(std::string islandName);

private:
	Loader();
	~Loader();

	static int texConvertFlags;

};