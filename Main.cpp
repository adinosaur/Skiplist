#include "Skiplist.h"
#include <stdint.h>
#include <stdio.h>

void print_val(void* v)
{
	printf("%ld, ", v);
}

int main()
{
	struct Skiplist* list = skiplist_create();

	skiplist_insert(list, (int64_t)21, (void*)21);
	skiplist_foreach(list, print_val);
	printf("\n");
	
	skiplist_insert(list, (int64_t)4, (void*)4);
	skiplist_foreach(list, print_val);
	printf("\n");

	skiplist_insert(list, (int64_t)67, (void*)67);
	skiplist_foreach(list, print_val);
	printf("\n");
	
	void* r1 = skiplist_search(list, (int64_t)4);
	void* r2 = skiplist_search(list, (int64_t)3);
	printf("%ld\n", r1);
	printf("%ld\n", r2);

	int64_t length = skiplist_length(list);
	printf("%ld\n", length);
	
	skiplist_destory(list);
	return 0;
}