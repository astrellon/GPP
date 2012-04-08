#pragma once

#include "bmp.h"
#include "jpeg.h"
#include "vector.h"

class FrameBuffer {
public:
	FrameBuffer() : width(0), height(0), data(NULL) {
	}
	FrameBuffer(int width, int height) : data(NULL) {
		setSize(width, height);
	}
	
	void setSize(int width, int height) {
		this->width = width;
		this->height = height;
		if (data == NULL) {
			delete [] data;
		}
		
		data = new Vector4f[width * height];
	}
	
	inline void setPixel(int x, int y, const Vector4f &pixel) {
		data[y * width + x] = pixel;
	}
	
	inline const Vector4f *getData() const {
		return data;
	}
	
	inline int getWidth() const {
		return width;
	}
	inline int getHeight() const {
		return height;
	}
	
	void save(const char *filename, const char *format) const {
		if (strcmp(format, "bmp") == 0) {
			saveBmp(filename);
		}
		else if (strcmp(format, "jpg") == 0 || strcmp(format, "jpeg") == 0) {
			saveJpeg(filename);
		}
		else {
			printf("Unknown format '%s'\n", format);
		}
	}
protected:
	
	void saveBmp(const char *filename) const {
		Bmp output(width, height);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				const Vector4f &p = data[y * width + x];
				int r = (p.x > 255.0f ? 255 : (p.x < 0.0f ? 0 : (int)p.x));
				int g = (p.y > 255.0f ? 255 : (p.y < 0.0f ? 0 : (int)p.y));
				int b = (p.z > 255.0f ? 255 : (p.z < 0.0f ? 0 : (int)p.z));
				output.setPixel(x, y, r, g, b);
			}
		}
		int result = output.save(filename);
		if (result != 1) {
			printf("Unable to save framebuff as bmp: %d\n", result);
		}
	}

	void saveJpeg(const char *filename) const {
		Jpeg output(width, height, 90.0f);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				const Vector4f &p = data[y * width + x];
				int r = (p.x > 255.0f ? 255 : (p.x < 0.0f ? 0 : (int)p.x));
				int g = (p.y > 255.0f ? 255 : (p.y < 0.0f ? 0 : (int)p.y));
				int b = (p.z > 255.0f ? 255 : (p.z < 0.0f ? 0 : (int)p.z));
				output.setPixel(x, y, r, g, b);
			}
		}
		int result = output.save(filename);
		if (result != 1) {
			printf("Unable to save framebuff as bmp: %d\n", result);
		}
	}
	int width;
	int height;
	Vector4f *data;
	
	Vector4f min_lum;
	Vector4f max_lum;
};
