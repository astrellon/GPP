#pragma once
#include <stdio.h>

#include "iimage.h"

typedef Pixel<unsigned char, 4> PngPixel;

class Png : public IImage<unsigned char, 4> {
public:
	Png() {
		width = height = 0;
		pixels = NULL;
	}
	Png(int width, int height) {
		pixels = NULL;
		setSize(width, height);
	}
	~Png() {
		destroy();
	}

	virtual void setPixel(int x, int y, const unsigned char &r, const unsigned char &g, const unsigned char &b, const unsigned char &a=1) {
		PngPixel &pixel = pixels[y * width + x];
		pixel.elements[0] = r;
		pixel.elements[1] = g;
		pixel.elements[2] = b;
		pixel.elements[3] = a;
	}

	virtual int save(const char *filename);
	virtual int load(const char *filename) {return -1;};

};

#ifndef _MSC_VER
#	include "png.cpp"
#endif
