#pragma once
#include <stdio.h>

typedef struct _pixel_t {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	
	_pixel_t() {
		r = g = b = 0;
	}
} pixel_t;

class Bmp {
public:
	Bmp(int width, int height) {
		this->width = width;
		this->height = height;
		
		pixels = new pixel_t*[width];
		for (int x = 0; x < width; x++) {
			pixels[x] = new pixel_t[height];
		}
	}
	~Bmp() {
		for (int x = 0; x < width; x++) {
			delete pixels[x];
		}
		delete pixels;
	}
	
	inline int getWidth() const {
		return width;
	}
	inline int getHeight() const {
		return height;
	}
	
	void setPixel(int x, int y, const pixel_t &pixel) {
		pixels[x][y] = pixel;
	}
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
		pixel_t &pixel = pixels[x][y];
		pixel.r = r;
		pixel.g = g;
		pixel.b = b;
	}
	pixel_t &getPixel(int x, int y) {
		return pixels[x][y];
	}
	
	void save(const char *filename) const {}
	void load(const char *filename) {}
	
private:
	int width;
	int height;
	
	pixel_t **pixels;
};
