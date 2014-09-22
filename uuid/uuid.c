#include <stdio.h>
#include <uuid/uuid.h>
#include <gd.h>

typedef struct {
	int r;
	int g;
	int b;
} color_t;

color_t pickColor(char i) {
	color_t color;
	color.r = (i & 0xE0) >> 5;
	color.g = (i & 0x1C) >> 2;
	color.b = i & 0x3;
}

int main(char *argv[], int argc) {
	int i;
	uuid_t sampleId;
	gdImagePtr image;
	FILE *imageFile;

	uuid_generate(sampleId);
	
	//"%08x-%04x-%04x-%04x-%012x\n"
	for(i = 0; i < 16; i++) {
		printf("%02x", sampleId[i]);
		if(i == 4 || i == 6 || i == 8 || i == 10)
			putchar('-');
	}
	putchar('\n');

	image = gdImageCreateTrueColor(128, 20);

	for(i = 0; i < 16; i++) {
		int r, g, b, color;
		r = (sampleId[i] & 0xE0) >> 5;
		g = (sampleId[i] & 0x1C) >> 2;
		b = sampleId[i] & 0x3;

		//printf("Byte: %02x R: %x G: %x B: %x\n", sampleId[i], r, g, b);

		color = gdImageColorResolve(image, r*32, g*32, b*64);

		gdImageFilledRectangle(image, i*8, 0, (i*8)+7, 19, color);
	}

	imageFile = fopen("uuid.png", "wb");
	gdImagePng(image, imageFile);
	fclose(imageFile);

	gdImageDestroy(image);

	return 0;
}
