#pragma once

#include <stdint.h>

typedef void(*FunType)(void*);

struct Skiplist;

struct Skiplist* skiplist_create();
void skiplist_destory(struct Skiplist* l);
void skiplist_insert(struct Skiplist* l, int64_t k, void* v);
void skiplist_remove(struct Skiplist* l, int64_t k);
void* skiplist_search(struct Skiplist* l, int64_t k);
void skiplist_foreach(struct Skiplist* l, FunType f);
uint64_t skiplist_length(struct Skiplist* l);