#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lzw.h"

char *convbin(uint16_t a);

uint16_t lzw_encoder_gencode(lzw_t *lz)
{
	pair_t p = {lz->raw_data + lz->cursor, 1};

	uint16_t *value = NULL;
	uint16_t *old_value = NULL;

	while((value = dict_find(lz->table, p)))
	{
		p.size++;
		lz->cursor++;
		old_value = value;
	}

	// We doin this because bro ain't no way
	if(lz->value <= 4095)
	{
		dict_insert(&lz->table, p, lz->value++);
		// lz->cursor--;
	}

	return *old_value;
}

uint16_t *lzw_encode(lzw_t *lz, size_t *size)
{
	uint16_t *stream = NULL;

	uint8_t code_size = lz->code_size;

	uint16_t code = 0;
	uint16_t packed = 0;

	uint16_t bitpos = 0;	// bit position in the new array
	uint8_t rem = 0;
	char k = 0;

	while(lz->cursor < lz->raw_data_size)
	{
		assert(!(bitpos && rem));
		if(rem)
		{
			packed |= code << bitpos;
			bitpos += rem;
			rem = 0;
		}

		// getting new code
		code = lzw_encoder_gencode(lz);

		//printf("%s ", convbin(code));

		if(code >= (1 << code_size))
			code_size++;

		// filling up the value
		packed |= code << bitpos;
		bitpos += code_size;

		k = 0;

		if(bitpos >= 16)
		{
			rem = bitpos % 16;
			code >>= code_size - rem;
			bitpos = 0;
			k = 1;
		}
		else
		{
			rem = 0;
		}

		if(k)
		{
			if(*size % 16 == 0)
				stream = realloc(stream, sizeof(uint16_t) * (*size + 16));
			stream[(*size)++] = packed;
			packed = 0;
		}
	}


	if(*size % 16 == 0)
		stream = realloc(stream, sizeof(uint16_t) * (*size + 16));
	stream[(*size)++] = packed;
	packed = 0;

	return stream;
}
