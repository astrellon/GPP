#pragma once
#include <stdio.h>

typedef struct _pixel_t {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	
	_pixel_t() {
		r = g = b = 200;
	}
} pixel_t;

class Bmp {
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
	
	void destroy() {
		if (pixels != NULL) {
			for (int y = 0; y < height; y++) {
				delete pixels[y];
			}
			delete pixels;
		}
		pixels = NULL;
	}
	
	inline int getWidth() const {
		return width;
	}
	inline int getHeight() const {
		return height;
	}
	
	void print(int x, int y) {
		pixel_t &p = getPixel(x, y);
		printf("Pixel at (%d, %d) [%d, %d, %d]\n", x, y, p.r, p.g, p.b);
	}
	
	void setPixel(int x, int y, const pixel_t &pixel) {
		pixels[y][x] = pixel;
	}
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
		pixel_t &pixel = pixels[y][x];
		pixel.r = r;
		pixel.g = g;
		pixel.b = b;
	}
	pixel_t &getPixel(int x, int y) {
		return pixels[y][x];
	}
	
	int save(const char *filename) const;
	int load(const char *filename);
	
private:
	int width;
	int height;
	
	void setSize(int width, int height) {
		destroy();
		
		this->width = width;
		this->height = height;
		
		pixels = new pixel_t*[height];
		for (int y = 0; y < height; y++) {
			pixels[y] = new pixel_t[width];
		}
	}
	
	pixel_t **pixels;
};

//#include "bmp.cpp"
