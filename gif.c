#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gif.h"
#include "lzw.h"

uint16_t GIF_CLEAR_CODE = 256;
uint16_t GIF_END_OF_INFO = 257;

void gif_write(image_t *img, const char *file)
{
	FILE *f = fopen(file, "wb+");

	fprintf(f, "GIF87a");

	fwrite(&img->width, sizeof img->width, 1, f);		// Image Logical Width
	fwrite(&img->height, sizeof img->height, 1, f);		// Image Logical height
	fwrite("\xf7", 1, 1, f);				// Image Global Color Table
	fwrite("\x00", 1, 1, f);				// Image Background color Index (for transpernecy)
	fwrite("\x00", 1, 1, f);				// Image Pixel Aspect Ratio

	fwrite(img->palette, sizeof img->palette, 1, f);	// Global Color Table

	fwrite("\x2C", 1, 1, f);					// Image Descriptor Separator
	fwrite("\x0000", 2, 1, f);				// Image Left Position 0
	fwrite("\x0000", 2, 1, f);				// Image Right Position 0
	fwrite(&img->width, sizeof img->width, 1, f);		// Image Width
	fwrite(&img->height, sizeof img->height, 1, f);		// Image height
	fwrite("\x00", 1, 1, f);				// Image Packed field for local colortable

	fwrite("\x08", 1, 1, f);				// LZW mimimum code size

	lzw_t lz = {
		.table = NULL,
		.code_size = 9,
		.min_code_size = 9,
		.raw_data = img->codes,
		.raw_data_size = img->width * img->height,
		.value = 256,
		.cursor = 0
	};	

	dict_insert(&lz.table, (pair_t){(uint8_t *)&GIF_CLEAR_CODE, 2}, lz.value++);
	dict_insert(&lz.table, (pair_t){(uint8_t *)&GIF_END_OF_INFO, 2}, lz.value++);

	size_t size = 0;
	uint8_t *codes = lzw_encode(&lz, &size);

	size_t i = 0, l = 0;
	for(i = 0; i <= size; i += 255)
	{
		uint8_t blksize = (size - l > 255 ? 255 : size - l);
		fwrite(&blksize, sizeof blksize, 1, f);

		for(int j = 0; j < 15; j++)
		{
			for(int k = 0; k < 17; k++)
			{
				if(l >= size)
				{
					goto OUT;
				}

				fwrite(codes + l, 1, 1, f);
				l++;
			}
		}
	}
OUT:

//	fwrite(&GIF_END_OF_INFO, sizeof GIF_END_OF_INFO, 1, f);

	fwrite("\x00", 1, 1, f);
	fwrite("\x3B", 1, 1, f);

	fclose(f);
}

void gif_infodump(image_t *img)
{
	printf("Signature: GIF89a\n");

	printf("Width: %hu\n", img->width);
	printf("Height: %hu\n", img->height);
	printf("Packed Field: %2x\n", 0xF3);
	printf("Background Color Index: %hu\n", 0);
	printf("Pixel Aspect Ratio: %hu\n", 0);

	printf("Global Color Table:\n");
	for(int i = 0; i < 256; i++)
		printf("%2x %2x %2x\n", img->palette[i][0], img->palette[i][1], img->palette[i][2]);

	printf("Image Separator: %x\n", 0x2C);
	printf("Image Left Position: %hu\n", 0);
	printf("Image Right Position: %hu\n", 0);
	printf("Image Width: %hu\n", img->width);
	printf("Image Height: %hu\n", img->height);
	printf("Packed Fields: %2x\n", 0);

	printf("LZW Minimum Code: %d\n", 8);

	lzw_t lz = {
		.table = NULL,
		.code_size = 9,
		.min_code_size = 9,
		.raw_data = img->codes,
		.raw_data_size = img->width * img->height,
		.value = 256,
		.cursor = 0,
	};	

	dict_insert(&lz.table, (pair_t){(uint8_t *)&GIF_CLEAR_CODE, 2}, lz.value++);
	dict_insert(&lz.table, (pair_t){(uint8_t *)&GIF_END_OF_INFO, 2}, lz.value++);

	size_t size = 0;
	uint8_t *codes = lzw_encode(&lz, &size);

	size_t i = 0, l = 0;
	for(i = 0; i <= size; i += 255)
	{
		printf("Block Size: %ld\n", size - l > 255 ? 255 : size - l);

		for(int j = 0; j < 15; j++)
		{
			for(int k = 0; k < 17; k++)
			{
				if(l >= size)
				{
					printf("%02X %02X\n", GIF_END_OF_INFO & 0xFF, GIF_END_OF_INFO >> 8);
					goto OUT;
				}
				printf("%02X ", codes[l++]);
			}
			puts("");
		}
	}
OUT:
	puts("");

	printf("Block Size: %d\n", 0);

	printf("Terminal Code: %2x\n", ';');
}
