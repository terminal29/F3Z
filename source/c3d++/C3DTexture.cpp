#include <c3d++/C3DTexture.h>
#include <iostream>

int C3DTexture::textureConvertFlags_ =
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) |
	 GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) |
	 GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

C3DTexture::C3DTexture() : C3DTexture(64, 64)
{
}

C3DTexture::C3DTexture(size_t x, size_t y)
{
	Pixel p(0, 0, 0, 255);
	std::vector<std::vector<Pixel>> bitmap(x, std::vector<Pixel>(y, p));
	setTexture(bitmap);
}

C3DTexture::C3DTexture(std::vector<std::vector<Pixel>> &bitmap)
{
	setTexture(bitmap);
}

C3DTexture::C3DTexture(const C3DTexture &that)
{
	setTexture(that.bitmap_);
}

C3DTexture C3DTexture::operator=(const C3DTexture &that)
{
	if (this != &that)
	{
		setTexture(that.bitmap_);
	}
	return *this;
}

void C3DTexture::setTexture(std::vector<std::vector<Pixel>> newBitmap)
{
	bitmap_ = newBitmap;
	bitmapSize_ = bitmap_.size();
	if (bitmapSize_ < 64)
	{
		Error::throwError("Textures must be 64x64 or larger"); // TODO Fix this
	}
	if (initialized_)
	{
		C3D_TexDelete(&texture_);
	}
	C3D_TexInit(&texture_, bitmapSize_, bitmapSize_, GPU_RGBA8);
	initialized_ = true;
	u8 *texDataLilEndian = (u8 *)linearAlloc(bitmapSize_ * bitmapSize_ * 4);

	u8 *texPtr = texDataLilEndian;
	for (size_t i = 0; i < bitmapSize_; i++)
	{
		for (size_t j = 0; j < bitmapSize_; j++)
		{
			*texPtr++ = bitmap_.at(i).at(j).a;
			*texPtr++ = bitmap_.at(i).at(j).b;
			*texPtr++ = bitmap_.at(i).at(j).g;
			*texPtr++ = bitmap_.at(i).at(j).r;
		}
	}

	// ensure data is in physical ram
	GSPGPU_FlushDataCache(texDataLilEndian, bitmapSize_ * bitmapSize_ * 4);

	// Convert image to 3DS tiled texture format
	C3D_SyncDisplayTransfer((u32 *)texDataLilEndian, GX_BUFFER_DIM(bitmapSize_, bitmapSize_), (u32 *)texture_.data, GX_BUFFER_DIM(bitmapSize_, bitmapSize_), textureConvertFlags_);

	// gspWaitForPPF();

	linearFree(texDataLilEndian);
}

C3D_Tex const &C3DTexture::getCTexture() const
{
	return texture_;
}

std::vector<std::vector<Pixel>> const &C3DTexture::getTexture() const
{
	return bitmap_;
}

C3DTexture::~C3DTexture()
{
	if (initialized_)
		C3D_TexDelete(&texture_);
}