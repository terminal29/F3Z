#pragma once
enum RenderTarget {
	RT_TOPLEFT = 0,
	RT_TOPRIGHT,
	RT_TOP,
	RT_BOTTOM,
	RT_ALL
};

struct Bitmap {
	unsigned char* pixels;
	unsigned int width = 0, height = 0, datalen = 0;
};

typedef struct StaticTile {
	/* If a tile's texture is allowed to be randomly rotated */
	bool canRotate = true;
	/* Tile's texture */
	C3D_Tex* tileTex;
} StaticTile;