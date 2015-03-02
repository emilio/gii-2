#include "binary-heap.h"

b_heap_t* b_heap_new() {
	b_heap_t* ret = (b_heap_t*) malloc(sizeof(b_heap_t));

	ret->size = ret->capacity = 0;

	ret->elements = NULL;

	return ret;
}

void b_heap_destroy(b_heap_t* heap) {
	if ( heap->elements )
		free(heap->elements);
	free(heap);
}

void b_heap_grow(b_heap_t* heap) {
	size_t target_capacity = heap->capacity == 0 ?
								BINARY_HEAP_INITIAL_CAPACITY
								: heap->capacity * 2;

	heap->elements = realloc(heap->elements, target_capacity * sizeof(b_heap_tuple_t));

	assert(heap->elements != NULL);
}

void b_heap_pop(b_heap_t* heap) {

	heap->size--;

	if ( heap->size == 0 )
		return;

	/**
	 * Move last element to the first position
	 * NOTE: Size is already decremented
	 */
	heap->elements[0] = heap->elements[heap->size];

	b_heap_heapify_from(heap, 0);
}

void b_heap_insert(b_heap_t* heap, b_heap_value_t value, b_heap_priority_t priority) {
	if ( heap->size == heap->capacity )
		b_heap_grow(heap);

	heap->elements[heap->size].value = value;
	heap->elements[heap->size].priority = priority;

	b_heap_heapify_to(heap, heap->size++);


}

void b_heap_heapify_to(b_heap_t* heap, size_t i) {
	b_heap_tuple_t temp;
	size_t parent_index;
	b_heap_tuple_t* elements = heap->elements;

	/** (i + 1) /2 = parent index */
	while ( i != 0 && elements[(parent_index = B_HEAP_PARENT_INDEX(i))].priority > elements[i].priority ) {
		/** TODO: Swapping by value, try to optimize */
		temp = elements[parent_index];
		elements[parent_index] = elements[i];
		elements[i] = temp;
		i = parent_index;
	}
}

void b_heap_heapify_from(b_heap_t* heap, size_t i) {
	size_t child_index;
	b_heap_tuple_t temp;

	b_heap_tuple_t* elements = heap->elements;

	/** Heapify */
	while ( i < heap->size ) {
		child_index = B_HEAP_LEFT_INDEX(i);

		if ( child_index >= heap->size )
			break;

		/** There are two children */
		if ( child_index + 1 < heap->size )
			if ( elements[child_index].priority > elements[child_index + 1].priority )
				child_index++;

		if ( elements[child_index].priority < elements[i].priority ) {
			/** swap(elements[child_index], elements[i] ) */
			temp = elements[child_index];
			elements[child_index] = elements[i];
			elements[i] = temp;
		}

		i = child_index;
	}
}

void b_heap_heapify(b_heap_t* heap) {
	size_t elements = heap->size / 2;

	while ( elements-- )
		b_heap_heapify_from(heap, elements);
}

