#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dict.h"

uint16_t *dict_find(dict_node_t *head, pair_t key)
{
	if(head == NULL)
		return NULL;

	int c = head->key.size - key.size;
	c = (c == 0 ? memcmp(head->key.str, key.str, key.size) : c);

	if(c == 0)
		return &head->value;
	else if(c < 0)
		return dict_find(head->left, key);
	else
		return dict_find(head->right, key);
}

static void rotate_left(dict_node_t **head)
{
	dict_node_t *old_head = head[0];
	dict_node_t *new_head = head[0]->right;
	dict_node_t *old_right_left = head[0]->right->left;

	head[0]->right = old_right_left;
	head[0] = new_head;
	new_head->left = old_head;

	if(head[0]->right)
		head[0]->right->height =
			(head[0]->right->left ? head[0]->right->left->height : 0)
			- (head[0]->right->right ? head[0]->right->right->height : 0);

	head[0]->height =
		(head[0]->left ? head[0]->left->height : 0)
			- (head[0]->right ? head[0]->right->height : 0);
}

static void rotate_right(dict_node_t **head)
{
	dict_node_t *old_head = head[0];
	dict_node_t *new_head = head[0]->left;
	dict_node_t *old_left_right = head[0]->left->right;

	head[0]->left = old_left_right;
	head[0] = new_head;
	new_head->right = old_head;

	if(head[0]->left)
		head[0]->left->height =
			(head[0]->left->left ? head[0]->left->left->height : 0)
			- (head[0]->left->right ? head[0]->left->right->height : 0);

	head[0]->height =
		(head[0]->left ? head[0]->left->height : 0)
			- (head[0]->right ? head[0]->right->height : 0);
}

int dict_insert(dict_node_t **head, pair_t key, uint16_t value)
{
	if(head[0] == NULL)
	{
		head[0] = calloc(sizeof(dict_node_t), 1);
		head[0]->key = key;
		head[0]->value = value;
		head[0]->height = 1;

		return 1;
	}

	int c = head[0]->key.size - key.size;
	c = (c == 0 ? memcmp(head[0]->key.str, key.str, key.size) : c);

	int k = 0;
	if(c == 0)
		return 1;
	else if(c < 0)
		k = dict_insert(&head[0]->left, key, value);
	else
		k = dict_insert(&head[0]->right, key, value);

	// Balancing act

	char left_height = head[0]->left ? head[0]->left->height : 0;
	char right_height = head[0]->right ? head[0]->right->height : 0;

	if(left_height - right_height < -1)
	{
		if(head[0]->right == NULL)
		{
			if(head[0]->right->left)
			{
				// weird juggle
				rotate_right(&head[0]->right);
				rotate_left(head);
			}
			else
			{
				// normal rotation
				rotate_left(head);
			}
		}
		else
			rotate_left(head);
	}
	else if(left_height - right_height > 1)
	{
		if(head[0]->right == NULL)
		{
			if(head[0]->left->right)
			{
				// weird juggle
				rotate_left(&head[0]->left);
				rotate_right(head);
			}
			else
			{
				// normal rotation
				rotate_right(head);
			}
		}
		else
			rotate_right(head);
	}
	else
	{
		head[0]->height = left_height > right_height ? left_height + 1 : right_height + 1;
	}

	return k;
}

void dict_print(dict_node_t *head, int depth)
{
	if(head == NULL) return;

	printf("%*c%.*s = %d\n", depth * 2, 0, (int)head->key.size, (char *)head->key.str, head->value);

	dict_print(head->left, depth + 1);
	dict_print(head->right, depth + 1);
}
