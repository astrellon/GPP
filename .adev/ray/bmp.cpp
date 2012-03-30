int Bmp::save(const char *filename) const {
	if (filename == NULL) {
		return -1;
	}
	FILE *output = fopen(filename, "wb");
	if (output == NULL) {
		return -2;
	}
	fwrite("BM", 1, 2, output);
	int padding = 4 - ((width * 3) % 4);
	if (padding >= 4) {
		padding = 0;
	}
	int pixelsize = ((width * 3) + padding) * height;
	int filesize = 54 + pixelsize;
	fwrite(&filesize, 4, 1, output);
	int reserved = 0;
	fwrite(&reserved, 4, 1, output);
	int offset = 54;
	fwrite(&offset, 4, 1, output);
	
	int headerSize = 40;
	fwrite(&headerSize, 4, 1, output);
	fwrite(&width, 4, 1, output);
	fwrite(&height, 4, 1, output);
	short planes = 1;
	fwrite(&planes, 2, 1, output);
	short bits = 24;
	fwrite(&bits, 2, 1, output);
	
	for(int i = 0, zero = 0; i < 6; i++) {
		fwrite(&zero, 4, 1, output);
	}
	
	int i = 0;
	for(int y = height - 1; y >= 0; y--) {
		const pixel_t *row = pixels[y];
		for (int x = 0; x < width; x++) {
			const pixel_t &p = row[x];
			fwrite(&p.b, 1, 1, output);
			fwrite(&p.g, 1, 1, output);
			fwrite(&p.r, 1, 1, output);
		}
		i += 3;
		int zero = 0;
		fwrite(&zero, padding, 1, output);
		i += padding;
	}
	fclose(output);
	return 1;
}

int Bmp::load(const char *filename) {
	if (filename == NULL) {
		return -1;
	}
	FILE *input = fopen(filename, "rb");
	if (input == NULL) {
		return -2;
	}
	
	char buff[3];
	fread(buff, 2, 1, input);
	if (buff[0] != 'B' && buff[1] != 'M') {
		fclose(input);
		return -3;
	}
	
	int filesize;
	fread(&filesize, 4, 1, input);
	int zero;
	fread(&zero, 4, 1, input);
	int offset;
	fread(&filesize, 4, 1, input);
	
	int headersize;
	fread(&headersize, 4, 1, input);
	int wid;
	fread(&wid, 4, 1, input);
	int hei;
	fread(&hei, 4, 1, input);
	int padding = 4 - ((wid * 3) % 4);
	if (padding >= 4) {
		padding = 0;
	}
	short planes;
	fread(&planes, 2, 1, input);
	short bits;
	fread(&bits, 2, 1, input);
	if (bits != 24) {
		fclose(input);
		return -4;
	}
	
	for(int i = 0, zero = 0; i < 6; i++) {
		fread(&zero, 4, 1, input);
	}
	
	setSize(wid, hei);
	for (int y = hei - 1; y >= 0; y--) {
		pixel_t *row = pixels[y];
		for (int x = 0; x < wid; x++) {
			pixel_t &p = row[x];
			fread(&p.b, 1, 1, input);
			fread(&p.g, 1, 1, input);
			fread(&p.r, 1, 1, input);
		}
		int zero = 0;
		fread(&zero, padding, 1, input);
	}
	
	fclose(input);
	return 1;
}
