#pragma once
#include "iimage.h"
#include <math.h>

#include <vector>

float round(const float x);

typedef Pixel<unsigned char, 3> JpegPixel;

typedef struct _JpegBitString {
	int len;
	int val;

	_JpegBitString() {
		len = val = 0;
	}
} JpegBitString;

class Jpeg : public IImage<unsigned char, 3> {
public:
	Jpeg(int width, int height, float quality);
	~Jpeg() {
		destroy();
	}

	virtual void setPixel(int x, int y, const unsigned char &r, const unsigned char &g, const unsigned char &b) {
		JpegPixel &pixel = pixels[y * width + x];
		pixel.elements[0] = r;
		pixel.elements[1] = g;
		pixel.elements[2] = b;
	}

	virtual int save(const char *filename);
	virtual int load(const char *filename) {return 0;};

private:
	float YTable[64];
	float UVTable[64];
	float fdtbl_Y[64];
	float fdtbl_UV[64];

	JpegBitString bitcode[65535];
	int category[65535];

	int DU[64];
	float YDU[64];
	float UDU[64];
	float VDU[64];

	JpegBitString YDC_HT [252];
	JpegBitString UVDC_HT[252];
	JpegBitString YAC_HT [252];
	JpegBitString UVAC_HT[252];

	FILE *output;

	int bytenew;
	int bytepos;

	void initQuantTables(int sf);
	void computeHuffmanTbl(JpegBitString *HT,
		const int nrcodes[17], const int *std_table);
	void initHuffmanTbl();
	void initCategoryNumber();
	void writeBits(const JpegBitString &bs);
	void writeByte(int value);
	void writeWord(int value);

	float *fDCTQuant(float data[64], const float fdtbl[64]);

	void writeAPP0();
	void writeSOF0(int width, int height);
	void writeDQT();
	void writeDHT();
	void writeSOS();
	float processDU(float CDU[64], 
		const float fdtbl[64], 
		float DC,
		JpegBitString *HTDC,
		JpegBitString *HTAC);

	void RGB2YUV(int xpos, int ypos);

	static const int ZigZag[64];
	static const int YQT[64];
	static const int UVQT[64];
	static const float aasf[8];

	static const int std_dc_luminance_nrcodes[17];
	static const int std_dc_luminance_values[12];
	static const int std_ac_luminance_nrcodes[17];
	static const int std_ac_luminance_values[162];

	static const int std_dc_chrominance_nrcodes[17];
	static const int std_dc_chrominance_values[12];
	static const int std_ac_chrominance_nrcodes[17];
	static const int std_ac_chrominance_values[162];

};

#ifndef _MSC_VER
#	include "jpeg.cpp"
#endif
