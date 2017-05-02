
#include "Skiplist.h"
#include <stdint.h>
#include <malloc.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#define SKIPLIST_LEVEL_MAX 32
#define SKIPLIST_KEY_MIN INT64_MIN
#define SKIPLIST_KEY_MAX INT64_MAX

struct SkillistNode
{
	int64_t key;
	void* val;
	uint8_t level;
	struct SkillistNode* forward[1];
};

static struct SkillistNode NIL = { SKIPLIST_KEY_MAX, NULL, 0, {NULL} };

struct Skiplist
{
	struct SkillistNode* header;
	uint64_t length;
	uint8_t level;
};

struct Skiplist* skiplist_create()
{
	size_t size = sizeof(struct SkillistNode) + sizeof(void*) * SKIPLIST_LEVEL_MAX;
	struct SkillistNode* node = (struct SkillistNode*) malloc(size);
	assert(node);
	node->key = SKIPLIST_KEY_MIN;
	node->val = NULL;
	node->level = SKIPLIST_LEVEL_MAX;
	for (int i = 0; i <= SKIPLIST_LEVEL_MAX; i++)
		node->forward[i] = &NIL;

	struct Skiplist* list = (struct Skiplist*) malloc(sizeof(struct Skiplist));
	assert(list);
	list->header = node;
	list->length = 0;
	list->level = 0;	
	return list;
}

void skiplist_destory(struct Skiplist* list)
{
	assert(list);
	struct SkillistNode *iter = list->header, *prev;
	while (iter != &NIL)
	{
		prev = iter;
		iter = iter->forward[0];
		free(prev);
	}
	free(list);
}

void* skiplist_search(struct Skiplist* list, int64_t k)
{
	assert(list);
	if (list->length == 0)
		return NULL;
	
	struct SkillistNode* x = list->header;
	for (int i = list->level; i >= 0; i--)
	{
		while (x->forward[i]->key < k)
			x = x->forward[i];
	}
	x = x->forward[0];
	return (x && x->key == k) ? x->val : NULL;
}

static inline uint8_t random_level()
{
	srand((unsigned int)time(NULL));
	uint8_t level = 0;
	while (rand() % 2 && level <= SKIPLIST_LEVEL_MAX)
		level += 1;
	return level;
}

void skiplist_insert(struct Skiplist* list, int64_t k, void* v)
{
	assert(list);
	assert(k != SKIPLIST_KEY_MIN && k != SKIPLIST_KEY_MAX);
	assert(v);

	struct SkillistNode* update[SKIPLIST_LEVEL_MAX+1];
	for (int i = 0; i <= SKIPLIST_LEVEL_MAX; i++)
		update[i] = &NIL;

	struct SkillistNode* x = list->header;
	for (int i = list->level; i >= 0; i--)
	{
		while (x->forward[i]->key < k)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];
	if (x->key == k)
	{
		x->val = v;
		return;
	}

	uint8_t level = random_level();
	size_t size = sizeof(struct SkillistNode) + sizeof(void*) * level;
	x = (struct SkillistNode*) malloc(size);
	assert(x);
	x->key = k;
	x->val = v;
	x->level = level;

	if (level > list->level)
	{
		for (int i = list->level + 1; i <= level; i++)
			update[i] = list->header;
		list->level = level;
	}
		
	list->length += 1;

	for (int i = 0; i <= level; i++)
	{
		x->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = x;
	}
}

void skiplist_remove(struct Skiplist* list, int64_t k)
{
	assert(list);
	assert(k != INT64_MIN);

	struct SkillistNode* update[SKIPLIST_LEVEL_MAX+1];
	for (int i = 0; i <= SKIPLIST_LEVEL_MAX; i++)
		update[i] = &NIL;

	struct SkillistNode* x = list->header;
	for (int i = list->level; i >= 0; i--)
	{
		while (x->forward[i]->key < k)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];
	if (x->key != k)
		return;

	for (int i = 0; i <= list->level; i++)
	{
		if (update[i]->forward[i] != x)
			break;
		update[i]->forward[i] = x->forward[i];
	}

	free(x);

	while (list->level > 0 &&
		list->header->forward[list->level] == &NIL)
	{
		list->level -= 1;
	}

	list->length -= 1;
}

void skiplist_foreach(struct Skiplist* list, FunType fun)
{
	assert(list);
	assert(fun);
	for (struct SkillistNode* iter = list->header; 
		iter != &NIL; iter = iter->forward[0])
	{
		fun(iter->val);
	}
}

uint64_t skiplist_length(struct Skiplist* list)
{
	assert(list);
	return list->length;
}