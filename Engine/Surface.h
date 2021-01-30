#pragma once
#include "Colors.h"
#include <string>
class Surface {

public:
	Surface(const std::string& filename);

	Surface(int width, int height);
	Surface(const Surface&);
	Surface& operator=(const Surface& rhs);
	~Surface();
	void PutPixel(int x, int y, Color c);
	Color GetPixel(int x, int y) const;
	int GetHeight() const;
	int GetWidth() const;

private:
	int width;
	int height;
	Color* pPixels = nullptr;
};