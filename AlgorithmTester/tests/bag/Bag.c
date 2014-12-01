#include "Bag.h"

Item * newItem(const char * name, size_t weight, size_t value) {
	Item * ret = (Item *) malloc(sizeof(Item));
	ret->name = name;
	ret->weight = weight;
	ret->value = value;
	return ret;
}

Bag * newBag(size_t capacity) {
	Bag * ret = (Bag *) malloc(sizeof(Bag));
	ret->used_capacity = 0; // cached fields
	ret->value = 0; // cached fields
	ret->capacity = capacity;
	ret->items = newVector();
	return ret;
}

void Bag_addItem(Bag *bag, Item *item) {
	return_if(bag->used_capacity + item->weight > bag->capacity);

	bag->used_capacity += item->weight;
	bag->value += item->value;

	Vector_push(bag->items, item);
}

int Bag_supports(Bag *bag, Item *item) {
	if ( bag->used_capacity + item->weight > bag->capacity ) {
		return 0;
	}
	return 1;
}

void Bag_destroy(Bag *bag) {
	Vector_destroy(bag->items);
	free(bag);
}
