#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"

char *values[] = {
	"heallo",
	"joemumhoe",
	"stupid",
	"bruh",
	"nahman",
	"keepme",
	"opline",
	"opline",
	"sucket",
	"suckek"
};

int main(void)
{
	dict_node_t *dict = NULL;

	int id = 0;
	for(size_t i = 0; i < sizeof values / sizeof values[0]; i++)
	{
		pair_t p = {(uint8_t *)values[i], strlen(values[i])};
		dict_insert(&dict, p, id++);


		if(i == sizeof values / sizeof values[0] -2)
			dict_print(dict, 0);
	}

	puts("");
	dict_print(dict, 0);

	return 0;
}
