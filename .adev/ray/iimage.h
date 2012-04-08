#pragma once

#include <stdlib.h>

template <class T, int size>
class Pixel {
public:
	Pixel() {
		for (int i = 0; i < size; i++) {
			elements[i] = static_cast<T>(0);
		}
	}
	T elements[size];
};

template <class T, int size>
class IImage {
public:
	IImage() {
		pixels = NULL;
	}
	virtual void destroy() {
		if (pixels != NULL) {
			delete []pixels;
		}
		pixels = NULL;
	}
	virtual void setPixel(int x, int y, const Pixel<T, size> &pixel) {
		pixels[y * width + x] = pixel;
	}
	virtual Pixel<T, size> getPixel(int x, int y) const {
		return pixels[y * width + x];
	}

	virtual int getWidth() const {
		return width;
	}
	virtual int getHeight() const {
		return height;
	}

	virtual int save(const char *filename) = 0;
	virtual int load(const char *filename) = 0;

protected:
	void setSize(int width, int height) {
		destroy();
		
		this->width = width;
		this->height = height;
		
		pixels = new Pixel<T, size>[width * height];
	}

	Pixel<T, size> *pixels;
	int width;
	int height;
};
