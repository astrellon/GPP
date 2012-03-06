#include <stdio.h>
#include "bmp.h"

int main() {
	Bmp bmp(6, 4);
	bmp.setPixel(0, 0, 10, 20, 30);
	
	
	printf("Done!\n");
	return 0;
};
