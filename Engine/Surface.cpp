#include "Surface.h"
#include <cassert>
#include "ChiliWin.h"
#include <fstream>

Surface::Surface(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	assert(file);
	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*> (&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*> (&bmInfoHeader), sizeof(bmInfoHeader));
	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32); //only deal with 24bit bitmaps - rgb = 3* 8bytes each
	assert(bmInfoHeader.biCompression == BI_RGB); // uncompressed rgb
		width = bmInfoHeader.biWidth;
		 //allow reverse files
		int yStart, yEnd, dy;
		if (bmInfoHeader.biHeight < 0)
		{

			height = -bmInfoHeader.biHeight;
			yStart = 0;
			yEnd = height;
			dy = 1;
		}
		else {
			height = bmInfoHeader.biHeight;
			yStart = height-1;
			yEnd = -1;
			dy = -1;
		}
	pPixels = new Color[width * height];

	file.seekg(bmFileHeader.bfOffBits);// offset from beginning of file to where pixel data starts
	 const int padding  = (4 - (width * 4) % 4) % 4;
	 const bool is32 = bmInfoHeader.biBitCount == 32;
	for (int y = yStart ;y != yEnd; y+=dy)
	{
		for (int x = 0; x < width; x++) {
			PutPixel(x, y, Color( file.get(), file.get(), file.get() ));
			if (is32) {
				file.seekg(1, std::ios::cur);// skip alpha
			}
		}
	}
	if (!is32) {//lines are always gonna be aligned with 32bit

	file.seekg(padding, std::ios::cur);
	}
}

Surface::Surface(int width, int height)
	:
	width(width), 
	height(height),
	pPixels(new Color[width*height])
{
}

Surface::Surface(const Surface& rhs)
	: Surface(rhs.width, rhs.height)
{
	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++)
	{
		pPixels[i] = rhs.pPixels[i];
	}
}

Surface& Surface::operator=(const Surface& rhs)
{
	width = rhs.width;
	height = rhs.height;
	delete[] pPixels;

	pPixels = new Color[width * height];
	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++)
	{
		pPixels[i] = rhs.pPixels[i];
	}
	return *this;
}

Surface::~Surface()
{
	delete[] pPixels;
	pPixels = nullptr;
}

void Surface::PutPixel(int x, int y, Color c)
{ 
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);

	pPixels[y * width + x] = c;
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	return pPixels[y * width + x];
}

int Surface::GetHeight() const
{
	return height;
}

int Surface::GetWidth() const
{
	return width;
}
