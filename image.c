#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "image.h"

image_t image_load(const char *file_name)
{
	image_t img = {0};

	static char buff[256] = {0};

	FILE *f = fopen(file_name, "r");

	fscanf(f, "%s", buff);
	if(strcmp(buff, "P6") != 0)
		return img;

	fscanf(f, "%hu", &img.width);
	fscanf(f, "%hu", &img.height);

	fscanf(f, "%s\n", buff);
	if(strcmp(buff, "255") != 0)
		return img;

	img.data = calloc(sizeof(*img.data), img.width * img.height);

	fread(img.data, img.width * img.height, sizeof(uint8_t) * 3, f);

	fclose(f);

	return img;
}

void image_genpalette(image_t *img)
{
	uint32_t len = 0;

	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				img->palette[len][0] = i * (256 / 8);
				img->palette[len][1] = j * (256 / 8);
				img->palette[len][2] = k * (256 / 4);
				len++;
			}
		}
	}
}

int quantizecolor(image_t *img, uint8_t color[3], char diff[3])
{
	int r = color[0] / 32;
	int g = color[1] / 32;
	int b = color[2] / 64;

	int indx = r * 32 + g * 4 + b;

	diff[0] = color[0] - img->palette[indx][0];
	diff[1] = color[1] - img->palette[indx][1];
	diff[2] = color[2] - img->palette[indx][2];

	color[0] = img->palette[indx][0];
	color[1] = img->palette[indx][1];
	color[2] = img->palette[indx][2];

	return indx;
}

void increment(uint8_t color[3], char diff[3], float f)
{
#define CLAMP(f) (f > 255 ? 255 : (f < 0 ? 0 : f))

	color[0] = CLAMP(color[0] + diff[0] * f);
	color[1] = CLAMP(color[1] + diff[1] * f);
	color[2] = CLAMP(color[2] + diff[2] * f);
}

//from: Floyd Stienberg dithering wikipedia
void image_dither(image_t *img)
{
	img->codes = calloc(sizeof(uint8_t), img->width * img->height);

	int k = 0;
	for(int i =0; i < img->height; i++)
	{
		for(int j =0; j < img->width; j++)
		{
			char diff[3];
			img->codes[k++] = quantizecolor(img, img->data[i * img->width + j], diff);

			if(j + 1 < img->width)
			{
				increment(img->data[i * img->width + j + 1], diff, 7.0/16);
			}

			if(i + 1 < img->height && j - 1 >= 0)
			{
				increment(img->data[(i + 1) * img->width + j - 1], diff, 3.0 / 16);
			}

			if(i + 1 < img->height)
			{
				increment(img->data[(i + 1) * img->width + j], diff, 5.0 / 16);
			}

			if(i + 1 < img->height && j + 1 < img->width)
			{
				increment(img->data[(i + 1) * img->width + j + 1], diff, 1.0 / 16);
			}
		}
	}
}

void image_write(image_t *img, const char *file_name)
{
	FILE *f = fopen(file_name, "w+");
	fprintf(f, "P6 %d %d 255\n", img->width, img->height);
	fwrite(img->data, img->width * img->height, sizeof(uint8_t) * 3, f);
	fclose(f);
}

