#pragma once
#include <stdio.h>

#include "iimage.h"

typedef Pixel<unsigned char, 3> BmpPixel;

class Bmp : public IImage<unsigned char, 3> {
public:
	Bmp() {
		width = height = 0;
		pixels = NULL;
	}
	Bmp(int width, int height) {
		pixels = NULL;
		setSize(width, height);
	}
	~Bmp() {
		destroy();
	}

	virtual void setPixel(int x, int y, const unsigned char &r, const unsigned char &g, const unsigned char &b) {
		BmpPixel &pixel = pixels[y * width + x];
		pixel.elements[0] = r;
		pixel.elements[1] = g;
		pixel.elements[2] = b;
	}

	virtual int save(const char *filename);
	virtual int load(const char *filename);

};

#ifndef _MSC_VER
#	include "bmp.cpp"
#endif
