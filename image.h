#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdint.h>

typedef struct
{
	uint16_t width;
	uint16_t height;
	uint8_t (*data)[3];

	// 8 x 8 x 4
	uint8_t (palette[256])[3];

	uint8_t *codes;
} image_t;

image_t image_load(const char *file_name);
void image_dither(image_t *img);
void image_write(image_t *img, const char *file_name);

void image_genpalette(image_t *img);

#endif
