#pragma once
#include <citro3d.h>
#include <vector>
#include <sstream>
#include <Log.h>

struct Pixel {
	unsigned char 
		r = 0, 
		g = 0, 
		b = 0, 
		a = 0;
	Pixel() {};
	Pixel(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_) 
		:r(r_), 
		g(g_), 
		b(b_), 
		a(a_)
	{ };
};

class C3DTexture {
public:

	/* Initialize a blank 1x1 texture */
	C3DTexture();

	/* Initialize a blank (x,y) sized texture */
	C3DTexture(size_t x, size_t y);

	/* Initialize a C3DTexture with the pixels in the 2D n by n bitmap (determined by the width of the first row) */
	C3DTexture(std::vector<std::vector<Pixel>>& bitmap);

	~C3DTexture();

	C3DTexture operator= (const C3DTexture& tex);

	C3DTexture(const C3DTexture &tex);

	/* Sets the texture to a new bitmap */
	void setTexture(std::vector<std::vector<Pixel>> newBitmap);

	/* Gets the current bitmap */
	std::vector<std::vector<Pixel>>& getTexture();

	C3D_Tex* getCTexture();

private:
	bool initialized_ = false;
	C3D_Tex texture_;
	std::vector<std::vector<Pixel>> bitmap_;
	size_t bitmapSize_ = 0;
	static int textureConvertFlags_;
};