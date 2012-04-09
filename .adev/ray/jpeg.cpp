#ifdef _MSC_VER
#	include "jpeg.h"
#endif

float round(const float x) {
	if (x < 0) {
		return static_cast<float>(static_cast<int>(x - 0.5f));
	}
	return static_cast<float>(static_cast<int>(x + 0.5f));
}

float max(float x, float y) {
	return x > y ? x : y;
}
float min(float x, float y) {
	return x < y ? x : y;
}

const int Jpeg::ZigZag[] = {
	 0, 1, 5, 6,14,15,27,28,
     2, 4, 7,13,16,26,29,42,
     3, 8,12,17,25,30,41,43,
     9,11,18,24,31,40,44,53,
	10,19,23,32,39,45,52,54,
	20,22,33,38,46,51,55,60,
	21,34,37,47,50,56,59,61,
	35,36,48,49,57,58,62,63 };

const int Jpeg::YQT[] = {
	16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68,109,103, 77,
    24, 35, 55, 64, 81,104,113, 92,
    49, 64, 78, 87,103,121,120,101,
    72, 92, 95, 98,112,100,103, 99 };

const int Jpeg::UVQT[] = {
	17, 18, 24, 47, 99, 99, 99, 99,
    18, 21, 26, 66, 99, 99, 99, 99,
    24, 26, 56, 99, 99, 99, 99, 99,
    47, 66, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99 };

const float Jpeg::aasf[] = {
	1.0f, 1.387039845f, 1.306562965f, 1.175875602f,
	1.0f, 0.785694958f, 0.541196100f, 0.275899379f };

const int Jpeg::std_dc_luminance_nrcodes[] = {0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0};
const int Jpeg::std_dc_luminance_values[] = {0,1,2,3,4,5,6,7,8,9,10,11};
const int Jpeg::std_ac_luminance_nrcodes[] = {0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d};
const int Jpeg::std_ac_luminance_values[] = {
	0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,
    0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,
    0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x08,
    0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0,
    0x24,0x33,0x62,0x72,0x82,0x09,0x0a,0x16,
    0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28,
    0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,
    0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
    0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
    0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
    0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
    0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
    0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,
    0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
    0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,
    0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,
    0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,
    0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,
    0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,
    0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,
	0xf9,0xfa };

const int Jpeg::std_dc_chrominance_nrcodes[] = {0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0};
const int Jpeg::std_dc_chrominance_values[] = {0,1,2,3,4,5,6,7,8,9,10,11};
const int Jpeg::std_ac_chrominance_nrcodes[] = {0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77};
const int Jpeg::std_ac_chrominance_values[] = {
	0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,
    0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,
    0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,
    0xa1,0xb1,0xc1,0x09,0x23,0x33,0x52,0xf0,
    0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,
    0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26,
    0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,
    0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,
    0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,
    0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,
    0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,
    0x79,0x7a,0x82,0x83,0x84,0x85,0x86,0x87,
    0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,
    0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,
    0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,
    0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,
    0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,
    0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,
    0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,
    0xea,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,
	0xf9,0xfa };

Jpeg::Jpeg(int width, int height, float quality) {
	setSize(width, height);
	bytenew = 0;
	bytepos = 7;

	if (quality <= 0.0f) {
		quality = 1.0f;
	}
	if (quality > 100.0f) {
		quality = 100.0f;
	}
	int sf = 0;
	if (quality < 50) {
		sf = static_cast<int>(5000.0f / quality);
	}
	else {
		sf = static_cast<int>(200.0f - quality * 2.0f);
	}

	initHuffmanTbl();
	initCategoryNumber();
	initQuantTables(sf);
}

void Jpeg::initQuantTables(int sf) {
	float t;
	int i;
	for (i = 0; i < 64; i++) {
		t = floor(static_cast<float>(YQT[i] * sf + 50) / 100.0f);
		if (t < 1.0f) {
			t = 1.0f;
		}
		else if (t > 255.0f) {
			t = 255.0f;
		}
		YTable[ZigZag[i]] = t;
	}
	for (i = 0; i < 64; i++) {
		t = floor(static_cast<float>(UVQT[i] * sf + 50) / 100.0f);
		if (t < 1.0f) {
			t = 1.0f;
		}
		else if (t > 255.0f) {
			t = 255.0f;
		}
		UVTable[ZigZag[i]] = t;
	}
	i = 0;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			fdtbl_Y[i]  = (1.0f / (YTable [ZigZag[i]] * aasf[row] * aasf[col] * 8.0f));
			fdtbl_UV[i] = (1.0f / (UVTable[ZigZag[i]] * aasf[row] * aasf[col] * 8.0f));
			i++;
		}
	}
}

void Jpeg::computeHuffmanTbl(JpegBitString *HT, const int nrcodes[17], const int *std_table) {
	int codevalue = 0;
	int pos_in_table = 0;
	for (int k = 1; k <= 16; k++) {
		for (int j = 1; j <= nrcodes[k]; j++) {
			JpegBitString bs;
			bs.val = codevalue;
			bs.len = k;

			size_t pos = std_table[pos_in_table];
			HT[pos] = bs;
			
			pos_in_table++;
			codevalue++;
		}
		codevalue *= 2;
	}
}

void Jpeg::initHuffmanTbl() {
	computeHuffmanTbl(YDC_HT, std_dc_luminance_nrcodes, std_dc_luminance_values);
	computeHuffmanTbl(UVDC_HT, std_dc_chrominance_nrcodes, std_dc_chrominance_values);
	computeHuffmanTbl(YAC_HT, std_ac_luminance_nrcodes, std_ac_luminance_values);
	computeHuffmanTbl(UVAC_HT, std_ac_chrominance_nrcodes, std_ac_chrominance_values);
}

void Jpeg::initCategoryNumber() {
	int nrlower = 1;
	int nrupper = 2;
	int nr;
	for (int cat = 1; cat <= 15; cat++) {
		for (nr = nrlower; nr < nrupper; nr++) {
			category[32767 + nr] = cat;
			JpegBitString bs;
			bs.len = cat;
			bs.val = nr;
			bitcode[32767 + nr] = bs;
		}
		for (nr = -(nrupper - 1); nr <= -nrlower; nr++) {
			category[32767 + nr] = cat;
			JpegBitString bs;
			bs.len = cat;
			bs.val = nrupper - 1 + nr;
			bitcode[32767 + nr] = bs;
		}
		nrlower <<= 1;
		nrupper <<= 1;
	}
}

void Jpeg::writeBits(const JpegBitString &bs) {
	int value = bs.val;
	int posval = bs.len - 1;
	while (posval >= 0) {
		if (value & static_cast<unsigned int>(1 << posval)) {
			bytenew |= static_cast<unsigned int>(1 << bytepos);
		}
		posval--;
		bytepos--;
		if (bytepos < 0) {
			if (bytenew == 0xFF) {
				writeByte(0xFF);
				writeByte(0);
			}
			else {
				writeByte(bytenew);
			}
			bytepos = 7;
			bytenew = 0;
		}
	}
}

void Jpeg::writeByte(int value) {
	fwrite(&value, 1, 1, output);
}

void Jpeg::writeWord(int value) {
	writeByte((value >> 8) & 0xFF);
	writeByte(value & 0xFF);
}

float *Jpeg::fDCTQuant(float data[64], const float fdtbl[64]) {
	float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp10, tmp11, tmp12, tmp13;
	float z1, z2, z3, z4, z5, z11, z13;
	int i;
	int dataOff = 0;
	for (i = 0; i < 8; i++) {
		tmp0 = data[dataOff + 0] + data[dataOff + 7];
        tmp7 = data[dataOff + 0] - data[dataOff + 7];
        tmp1 = data[dataOff + 1] + data[dataOff + 6];
        tmp6 = data[dataOff + 1] - data[dataOff + 6];
        tmp2 = data[dataOff + 2] + data[dataOff + 5];
        tmp5 = data[dataOff + 2] - data[dataOff + 5];
        tmp3 = data[dataOff + 3] + data[dataOff + 4];
        tmp4 = data[dataOff + 3] - data[dataOff + 4];

		/* Even part */
        tmp10 = tmp0 + tmp3;    /* phase 2 */
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

		data[dataOff + 0] = tmp10 + tmp11; /* phase 3 */
        data[dataOff + 4] = tmp10 - tmp11;
        
        z1 = (tmp12 + tmp13) * 0.707106781f; /* c4 */
        data[dataOff + 2] = tmp13 + z1; /* phase 5 */
        data[dataOff + 6] = tmp13 - z1;
        
        /* Odd part */
        tmp10 = tmp4 + tmp5; /* phase 2 */
        tmp11 = tmp5 + tmp6;
        tmp12 = tmp6 + tmp7;
        
        /* The rotator is modified from fig 4-8 to avoid extra negations. */
        z5 = (tmp10 - tmp12) * 0.382683433f; /* c6 */
        z2 = 0.541196100f * tmp10 + z5; /* c2-c6 */
        z4 = 1.306562965f * tmp12 + z5; /* c2+c6 */
        z3 = tmp11 * 0.707106781f; /* c4 */
        
        z11 = tmp7 + z3;        /* phase 5 */
        z13 = tmp7 - z3;
        
        data[dataOff + 5] = z13 + z2;     /* phase 6 */
        data[dataOff + 3] = z13 - z2;
        data[dataOff + 1] = z11 + z4;
        data[dataOff + 7] = z11 - z4;
        
        dataOff += 8; /* advance pointer to next row */
	}

	/* Pass 2: process columns. */
	dataOff = 0;
	for (i = 0; i < 8; i++) {
		tmp0 = data[dataOff +  0] + data[dataOff + 56];
		tmp7 = data[dataOff +  0] - data[dataOff + 56];
		tmp1 = data[dataOff +  8] + data[dataOff + 48];
		tmp6 = data[dataOff +  8] - data[dataOff + 48];
		tmp2 = data[dataOff + 16] + data[dataOff + 40];
		tmp5 = data[dataOff + 16] - data[dataOff + 40];
		tmp3 = data[dataOff + 24] + data[dataOff + 32];
		tmp4 = data[dataOff + 24] - data[dataOff + 32];
        
		/* Even part */
		tmp10 = tmp0 + tmp3;    /* phase 2 */
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;
        
		data[dataOff+ 0] = tmp10 + tmp11; /* phase 3 */
		data[dataOff+32] = tmp10 - tmp11;
        
		z1 = (tmp12 + tmp13) * 0.707106781f; /* c4 */
		data[dataOff+16] = tmp13 + z1; /* phase 5 */
		data[dataOff+48] = tmp13 - z1;
        
		/* Odd part */
		tmp10 = tmp4 + tmp5; /* phase 2 */
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;
        
		/* The rotator is modified from fig 4-8 to avoid extra negations. */
		z5 = (tmp10 - tmp12) * 0.382683433f; /* c6 */
		z2 = 0.541196100f * tmp10 + z5; /* c2-c6 */
		z4 = 1.306562965f * tmp12 + z5; /* c2+c6 */
		z3 = tmp11 * 0.707106781f; /* c4 */
        
		z11 = tmp7 + z3;        /* phase 5 */
		z13 = tmp7 - z3;
        
		data[dataOff + 40] = z13 + z2; /* phase 6 */
		data[dataOff + 24] = z13 - z2;
		data[dataOff +  8] = z11 + z4;
		data[dataOff + 56] = z11 - z4;
        
		dataOff++; /* advance pointer to next column */
	}
        
	// Quantize/descale the coefficients
	for (i = 0; i < 64; i++) {
		// Apply the quantization and scaling factor & Round to nearest integer
		data[i] = round((data[i] * fdtbl[i]));
	}
	return data;
}

void Jpeg::writeAPP0() {
	writeWord(0xFFE0); // marker
    writeWord(16); // length
    writeByte(0x4A); // J
    writeByte(0x46); // F
    writeByte(0x49); // I
    writeByte(0x46); // F
    writeByte(0); // = "JFIF",'\0'
    writeByte(1); // versionhi
    writeByte(1); // versionlo
    writeByte(0); // xyunits
    writeWord(1); // xdensity
    writeWord(1); // ydensity
    writeByte(0); // thumbnwidth
    writeByte(0); // thumbnheight
}

void Jpeg::writeSOF0(int width, int height) {
	writeWord(0xFFC0); // marker
    writeWord(17);   // length, truecolor YUV JPG
    writeByte(8);    // precision
    writeWord(height);
    writeWord(width);
    writeByte(3);    // nrofcomponents
    writeByte(1);    // IdY
    writeByte(0x11); // HVY
    writeByte(0);    // QTY
    writeByte(2);    // IdU
    writeByte(0x11); // HVU
    writeByte(1);    // QTU
    writeByte(3);    // IdV
    writeByte(0x11); // HVV
    writeByte(1);    // QTV
}

void Jpeg::writeDQT() {
	writeWord(0xFFDB); // marker
    writeWord(132);    // length
    writeByte(0);
    int i;
    for (i=0; i<64; i++) {
		writeByte(static_cast<int>(YTable[i]));
    }
    writeByte(1);
    for (i=0; i<64; i++) {
		writeByte(static_cast<int>(UVTable[i]));
    }
}

void Jpeg::writeDHT() {
	writeWord(0xFFC4); // marker
    writeWord(0x01A2); // length
    int i;
        
    writeByte(0); // HTYDCinfo
    for (i = 0; i < 16; i++) {
		writeByte(std_dc_luminance_nrcodes[i + 1]);
    }
    for (i = 0; i <= 11; i++) {
		writeByte(std_dc_luminance_values[i]);
    }
        
    writeByte(0x10); // HTYACinfo
    for (i = 0; i < 16; i++) {
		writeByte(std_ac_luminance_nrcodes[i + 1]);
    }
    for (i = 0; i <= 161; i++) {
		writeByte(std_ac_luminance_values[i]);
    }
        
    writeByte(1); // HTUDCinfo
    for (i = 0; i < 16; i++) {
		writeByte(std_dc_chrominance_nrcodes[i + 1]);
    }
    for (i = 0; i <= 11; i++) {
		writeByte(std_dc_chrominance_values[i]);
    }
        
    writeByte(0x11); // HTUACinfo
    for (i = 0; i < 16; i++) {
		writeByte(std_ac_chrominance_nrcodes[i + 1]);
    }
    for (i = 0; i <= 161; i++) {
		writeByte(std_ac_chrominance_values[i]);
    }
}

void Jpeg::writeSOS() {
	writeWord(0xFFDA); // marker
    writeWord(12); // length
    writeByte(3); // nrofcomponents
    writeByte(1); // IdY
    writeByte(0); // HTY
    writeByte(2); // IdU
    writeByte(0x11); // HTU
    writeByte(3); // IdV
    writeByte(0x11); // HTV
    writeByte(0); // Ss
    writeByte(0x3f); // Se
    writeByte(0); // Bf
}

float Jpeg::processDU(float CDU[64], 
	const float fdtbl[64], 
	float DC, 
	//const std::vector<JpegBitString> &HTDC, 
	//const std::vector<JpegBitString> &HTAC) 
	JpegBitString *HTDC,
	JpegBitString *HTAC)
{
	const JpegBitString &EOB = HTAC[0x00];
	const JpegBitString &M16zeroes = HTAC[0xF0];
	int i;

	float *DU_DCT = fDCTQuant(CDU, fdtbl);
	for (i = 0; i < 64; i++) {
		DU[ZigZag[i]] = static_cast<int>(DU_DCT[i]);
	}

	int Diff = static_cast<int>(DU[0] - DC);
	DC = static_cast<float>(DU[0]);
	if (Diff == 0) {
		writeBits(HTDC[0]);
	}
	else {
		writeBits(HTDC[category[32767 + Diff]]);
		writeBits(bitcode[32767 + Diff]);
	}

	int end0pos = 63;
	for (; (end0pos > 0) && (DU[end0pos] == 0); end0pos--) {};

	if (end0pos == 0) {
		writeBits(EOB);
		return DC;
	}
	i = 1;
	while (i <= end0pos) {
		int startpos = i;
		for (; (DU[i] == 0) && (i <= end0pos); i++) {};
		int nrzeroes = i - startpos;
		if (nrzeroes >= 16) {
			for (int nrmarker = 1; nrmarker <= nrzeroes / 16; nrmarker++) {
				writeBits(M16zeroes);
			}
			nrzeroes = static_cast<int>(nrzeroes & 0xF);
		}
		const JpegBitString &bs = HTAC[nrzeroes * 16 + category[32767 + DU[i]]];
		//printf("HTAC: %d, %d\n", bs.len, bs.val);
		writeBits(bs);
		writeBits(bitcode[32767 + DU[i]]);
		i++;
	}
	if (end0pos != 63) {
		writeBits(EOB);
	}
	return DC;
}

void Jpeg::RGB2YUV(int xpos, int ypos) {
	int pos = 0;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			const JpegPixel &pixel = pixels[(ypos + y) * width + xpos + x];
			float R = 0;
			float G = 0;
			float B = 0;
			if (ypos + y < height && xpos + x < width) {
				R = static_cast<float>(pixel.elements[0]);
				G = static_cast<float>(pixel.elements[1]);
				B = static_cast<float>(pixel.elements[2]);
			}
			YDU[pos]=((( 0.29900f) * R + ( 0.58700f) * G + ( 0.11400f) * B)) - 128.0f;
            UDU[pos]=(((-0.16874f) * R + (-0.33126f) * G + ( 0.50000f) * B));
            VDU[pos]=((( 0.50000f) * R + (-0.41869f) * G + (-0.08131f) * B));
			pos++;
		}
	}
}

int Jpeg::save(const char *filename) {
	output = fopen(filename, "wb");
	if (output == NULL) {
		return -2;
	}

	bytenew = 0;
	bytepos = 7;

	writeWord(0xFFD8);
	writeAPP0();
	writeDQT();
	writeSOF0(width, height);
	writeDHT();
	writeSOS();

	float DCY = 0.0f;
	float DCU = 0.0f;
	float DCV = 0.0f;
	bytenew = 0;
	bytepos = 7;

	for (int ypos = 0; ypos < height; ypos += 8) {
		for (int xpos = 0; xpos < width; xpos += 8) {
			RGB2YUV(xpos, ypos);
			DCY = processDU(YDU, fdtbl_Y, DCY, YDC_HT, YAC_HT);
			DCU = processDU(UDU, fdtbl_UV, DCU, UVDC_HT, UVAC_HT);
			DCV = processDU(VDU, fdtbl_UV, DCV, UVDC_HT, UVAC_HT);
		}
	}

	if (bytepos >= 0) {
		JpegBitString fill;
		fill.len = bytepos + 1;
		fill.val = (1 << (bytepos + 1)) - 1;
		writeBits(fill);
	}

	writeWord(0xFFD9);

	fclose(output);
}
