#ifndef _LZW_H_
#define _LZW_H_

#include <stdint.h>
#include "dict.h"

typedef struct
{
	dict_node_t *table;		// LZW lookup table
	uint8_t code_size;		// size of the minimum code
	uint8_t min_code_size;

	uint8_t *raw_data;		// raw uncompressed data
	uint32_t raw_data_size;

	uint16_t value;			// maximum can be 4095, when hit we stop adding things to the table
	uint32_t cursor;		// where in the compression we are currently at
} lzw_t;

uint16_t lzw_encoder_gencode(lzw_t *lz);
uint8_t *lzw_encode(lzw_t *lz, size_t *s);

void lzw_decode_nopack(lzw_t *lz, uint16_t *,size_t s);

#endif
