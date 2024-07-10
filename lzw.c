#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lzw.h"

char *convbin(unsigned int a, int);

extern uint16_t GIF_CLEAR_CODE;
extern uint16_t GIF_END_OF_INFO;

uint16_t lzw_encoder_gencode(lzw_t *lz)
{
	if(lz->cursor >= lz->raw_data_size)
	{
		lz->cursor++;
		return GIF_END_OF_INFO;
	}

	if(lz->value == (1 << 12))
	{
		dict_clear(lz->table);
		lz->table = 0;
		lz->value = 256;

		lz->code_size = lz->min_code_size;

		dict_insert(&lz->table, (pair_t){(uint8_t *)&GIF_CLEAR_CODE, 2}, lz->value++);
		dict_insert(&lz->table, (pair_t){(uint8_t *)&GIF_END_OF_INFO, 2}, lz->value++);

		return GIF_CLEAR_CODE;
	}

	pair_t p = {lz->raw_data + lz->cursor, 1};

	int16_t value = 0;
	int16_t old_value = 0;

	while((value = dict_find(lz->table, p)) >= 0)
	{
		p.size++;
		lz->cursor++;
		old_value = value;
	}

	if(lz->value < (1 << 12) - 1)
	{
		dict_insert(&lz->table, p, lz->value++);
	}

	return old_value;
}

uint8_t *lzw_encode(lzw_t *lz, size_t *size)
{
	uint16_t *stream = NULL;
	int count = 0;

#define INSERT(a)\
	do {\
		if(count % 16 == 0)\
			stream = realloc(stream, sizeof(uint16_t) * (count + 16));\
		stream[count++] = (a);\
	} while(0);

	int bitpos = 0;

	uint16_t packing = 0;
	uint16_t code = 0;

	while(lz->cursor <= lz->raw_data_size)
	{
		code = lzw_encoder_gencode(lz);	

		printf("%s", convbin(code, lz->code_size));

		packing |= code << bitpos;
		bitpos += lz->code_size;

		if(bitpos >= 16)
		{
			//printf("p:%s \n", convbin(packing, 16));
			INSERT(packing);

			bitpos -= 16;		         // re-evaluate bitopose
			code >>= lz->code_size - bitpos; // drop already read portion
			packing = code;
			//printf("p:%s \n", convbin(packing, 16));
		}

		if(lz->value == (1 << lz->code_size) + 1)
			lz->code_size++;
	}

	if(bitpos > 0)
		INSERT(packing);

#undef INSERT

	*size = count * 2;
	return (uint8_t *)stream;
}

// LSB first then
char *convbin(unsigned int a, int code_size)
{
	assert(code_size < 32);
	static char bin[32];

	/*
	sprintf(bin, "%d ", a);
	return bin;
	*/

	int i = 0;
	while(i < code_size)
	{
		bin[i] = (a & 1) + '0';
		a >>= 1;
		i++;
	}
	bin[i] = 0;
	
	return bin;
}
