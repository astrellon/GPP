#include "png.h"

int Png::save(const char *filename) {
	if (filename == NULL) {
		return -1;
	}
	FILE *output = fopen(filename, "wb");
	if (output == NULL) {
		return -2;
	}
	const int pngsig1 = 0x89504e47;
	const int pngsig2 = 0x0D0A1A0A;
	fwrite(&pngsig1, sizeof(pngsig1), 1, output);
	fwrite(&pngsig2, sizeof(pngsig2), 1, output);

	fwrite(&width, sizeof(width), 1, output);
	fwrite(&height, sizeof(height), 1, output);
	const unsigned int rgb32bit = 0x08060000;
	fwrite(&rgb32bit, sizeof(rgb32bit), 1, output);

	const unsigned char zero = 0;
	fwrite(&zero, 1, 1, output);

	return -1;
}