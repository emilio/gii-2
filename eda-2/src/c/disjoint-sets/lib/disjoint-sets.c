#include "disjoint-sets.h"

disjoint_sets_t* disjoint_sets_new() {
	return disjoint_sets_new_with_capacity(0);
}

disjoint_sets_t* disjoint_sets_new_with_capacity(size_t capacity) {
	disjoint_sets_t* ret = (disjoint_sets_t*) malloc(sizeof(disjoint_sets_t));

	if ( ! ret )
		return NULL;

	ret->size = 0;
	ret->capacity = capacity;

	if ( capacity == 0 ) {
		ret->sets = NULL;
	} else {
		ret->sets = (disjoint_set_t*) malloc(capacity * sizeof(disjoint_set_t));

		if ( ! ret->sets ) {
			free(ret);
			return NULL;
		}
	}

	return ret;
}

disjoint_sets_t* disjoint_sets_new_with_size(size_t size) {
	disjoint_sets_t* ret = disjoint_sets_new_with_capacity(size);
	size_t i;

	if ( ! ret )
		return NULL;

	ret->size = size;

	for ( i = 0; i < size; ++i )
		ret->sets[i] = i;

	return ret;
}

void disjoint_sets_grow(disjoint_sets_t* self) {
	size_t target_capacity = self->capacity == 0 ?
								DISJOINT_SETS_INITIAL_CAPACITY
								: self->capacity * 2;

	if ( self->sets == NULL ) {
		self->sets = (disjoint_set_t*) malloc(target_capacity * sizeof(disjoint_set_t));
	} else {
		self->sets = (disjoint_set_t*) realloc(self->sets, target_capacity * sizeof(disjoint_set_t));
	}

	assert(self->sets != NULL);

	self->capacity = target_capacity;
}

disjoint_set_t disjoint_sets_add_set(disjoint_sets_t* self) {
	if ( self->size == self->capacity )
		disjoint_sets_grow(self);

	self->sets[self->size] = self->size;

	self->size++;

	return self->size - 1;
}

disjoint_set_t disjoint_sets_root_of(disjoint_sets_t* self, disjoint_set_t set) {
	assert(set < self->size);

	if ( self->sets[set] == set )
		return set;

	/** Path compression */
	self->sets[set] = disjoint_sets_root_of(self, self->sets[set]);

	return self->sets[set];
}

int disjoint_sets_are_same_set(disjoint_sets_t* self, disjoint_set_t set_1, disjoint_set_t set_2) {
	return disjoint_sets_root_of(self, set_1) == disjoint_sets_root_of(self, set_2);
}

void disjoint_sets_join(disjoint_sets_t* self, disjoint_set_t set_1, disjoint_set_t set_2) {
	/**
	 * Height optimization should be a good thing to do here,
	 * but since we use path compression, it shouldn't be a big
	 * issue.
	 */
	self->sets[disjoint_sets_root_of(self, set_1)] = disjoint_sets_root_of(self, set_2);
}

void disjoint_sets_destroy(disjoint_sets_t* self) {
	free(self->sets);
	free(self);
}
