#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "lzw.h"

#include "image.h"

char *convbin(unsigned int a, int);

int main(void)
{
	/*
	image_t i = image_load("tests/test.ppm");
	image_genpalette(&i);
	image_dither(&i);
	image_write(&i, "tests/out.ppm");
	*/

	static uint8_t testbed[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

	lzw_t lz = {
		.table = 0,
		.code_size = 7,
		.raw_data = testbed,
		.raw_data_size = sizeof testbed,
		.value = 128,
		.cursor = 0
	};

	size_t s = 0;
	uint8_t *values = lzw_encode(&lz, &s);

	puts("");
	for(size_t i = 0; i < s; i++)
		printf("%s", convbin(values[i], 8));
	puts("");
}
