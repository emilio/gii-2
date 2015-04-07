#ifndef DISJOINT_SET_H_
#define DISJOINT_SET_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DISJOINT_SETS_INITIAL_CAPACITY 10

typedef size_t disjoint_set_t;

/** Disjoint sets data structure */
typedef struct disjoint_sets {
	disjoint_set_t* sets;
	size_t capacity;
	size_t size;
} disjoint_sets_t;


/** Create a new, empty set of disjoint sets */
disjoint_sets_t* disjoint_sets_new();

/**
 * Create a new, empty set of disjoint sets, with capacity for
 * `capacity` sets
 */
disjoint_sets_t* disjoint_sets_new_with_capacity(size_t capacity);

/** Create a new, initialized set of `size` disjoint sets */
disjoint_sets_t* disjoint_sets_new_with_size(size_t size);

/** Grow to allow more sets */
void disjoint_sets_grow(disjoint_sets_t* self);

/** Add a new set */
disjoint_set_t disjoint_sets_add_set(disjoint_sets_t* self);

/** Find the root set in a set of sets */
disjoint_set_t disjoint_sets_root_of(disjoint_sets_t* self, disjoint_set_t set);

/** Check if two sets are the same */
int disjoint_sets_are_same_set(disjoint_sets_t* self, disjoint_set_t set_1, disjoint_set_t set_2);

/** Join two sets */
void disjoint_sets_join(disjoint_sets_t* self, disjoint_set_t set_1, disjoint_set_t set_2);

/** Destroy the sets and free memory */
void disjoint_sets_destroy(disjoint_sets_t* self);

#endif
