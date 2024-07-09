#ifndef _DICT_H_
#define _DICT_H_

#include <stdint.h>

typedef struct
{
	uint8_t *str;
	size_t size;
} pair_t;

typedef struct dict_node_t
{
	pair_t key;
	uint16_t value;
	uint32_t height;
	struct dict_node_t *left;
	struct dict_node_t *right;
} dict_node_t;

int16_t dict_find(dict_node_t *head, pair_t key);
int dict_insert(dict_node_t **head, pair_t key, uint16_t value);

void dict_print(dict_node_t *head, int depth);

#endif
