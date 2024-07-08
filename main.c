#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "lzw.h"

uint8_t value[] = "TO BE OR NOT TO BE OR TO BE OR NOT TO BE OR NOT TO BE#";

char *convbin(uint16_t a)
{
	static char bin[17] = {0};

	int i = 0;
	while(i < 16)
	{
		bin[i] = (a & 1) + '0';
		a >>= 1;
		i++;
	}
	
	return bin;
}

int main(void)
{
	uint8_t alph[] = "# ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	lzw_t lz = {
		.table = NULL,
		.code_size = 5,
		.raw_data = value,
		.raw_data_size = strlen(value),
		.value = 0,
		.cursor = 0
	};

	for(size_t i = 0; i < sizeof alph / sizeof alph[0] - 1; i++)
		dict_insert(&lz.table, (pair_t){alph + i, 1}, lz.value++);

	size_t s = 0;
	uint16_t *codes = lzw_encode(&lz, &s);

//	puts("");

	for(size_t i = 0; i < s; i++)
	{
		printf("%s", convbin(codes[i]));
	}
	puts("");

	dict_print(lz.table, 0);

	return 0;
}
