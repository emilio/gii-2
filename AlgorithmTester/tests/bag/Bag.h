#ifndef __BAG_H
#define __BAG_H
#include "Vector.h"

typedef struct Item {
	const char *name; // Simplifying
	size_t weight;
	size_t value;
} Item;

typedef struct Bag {
	size_t capacity;
	size_t used_capacity;
	size_t value;
	Vector * items;
} Bag;

Item * newItem(const char * name, size_t weight, size_t value);

Bag * newBag(size_t);

void Bag_addItem(Bag *, Item *);
int Bag_supports(Bag *, Item *);
void Bag_destroy(Bag *);
#endif
