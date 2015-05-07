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

// Ensure child i is in a correct position, via bubbling it up
void b_heap_heapify_to(b_heap_t* heap, size_t i) {
	b_heap_tuple_t temp;
	size_t parent_index;
	b_heap_tuple_t* elements = heap->elements;

	/** (i + 1) /2 = parent index */
	while ( i != 0 ) {
        parent_index = B_HEAP_PARENT_INDEX(i);

        // we're done
        if ( elements[parent_index].priority < elements[i].priority )
            break;

        // Else swap and continue
		temp = elements[parent_index];
		elements[parent_index] = elements[i];
		elements[i] = temp;
		i = parent_index;
	}
}

// Ensure element i ends up in a correct position,
// via bubbling it down to the bottom
void b_heap_heapify_from(b_heap_t* heap, size_t i) {
	size_t child_index;
	b_heap_tuple_t temp;

	b_heap_tuple_t* elements = heap->elements;

	/** Heapify */
	while ( i < heap->size ) {
		child_index = B_HEAP_LEFT_INDEX(i);

		if ( child_index >= heap->size )
			break;

		/** There are two children, pick the one with less priority and bubble it up */
		if ( child_index + 1 < heap->size )
			if ( elements[child_index].priority > elements[child_index + 1].priority )
				child_index++;

        // If we don't have to swap, we're done
		if ( elements[child_index].priority > elements[i].priority )
            break;

        // else swap and continue iterating
        /** swap(elements[child_index], elements[i] ) */
        temp = elements[child_index];
        elements[child_index] = elements[i];
        elements[i] = temp;

		i = child_index;
	}
}

/** Heapify: Ensure we meet the conditions */
void b_heap_heapify(b_heap_t* heap) {
	size_t elements = heap->size / 2;

	while ( elements-- )
		b_heap_heapify_from(heap, elements);
}

/** Set priority of element at the given index */
void b_heap_set_priority(b_heap_t* heap, size_t index, b_heap_priority_t new_priority) {
	b_heap_priority_t old_priority;

	assert(index < heap->size);

	old_priority = heap->elements[index].priority;
	heap->elements[index].priority = new_priority;

	if ( new_priority < old_priority )
		b_heap_heapify_to(heap, index);
	else
		b_heap_heapify_from(heap, index);
}

/** Change priority of element at the given index */
void b_heap_change_priority(b_heap_t* heap, size_t index, b_heap_priority_t delta) {
	return b_heap_set_priority(heap, index, b_heap_get_priority(heap, index) + delta);
}

/** Note before using this we must store heap.size! */
b_heap_value_t* b_heap_sort(b_heap_t* heap) {
	b_heap_value_t* ret = (b_heap_value_t*) malloc(b_heap_size(heap) * sizeof(b_heap_value_t));
	size_t i = 0;

	while ( ! b_heap_empty(heap) ) {
		ret[i++] = b_heap_front(heap);
		b_heap_pop(heap);
	}

	return ret;
}

/** Return the ordered heap: We extract as before, but with priority, and later reinsert */
void b_heap_heapsorted(b_heap_t* heap) {
	b_heap_tuple_t* temp = (b_heap_tuple_t*) malloc(b_heap_size(heap) * sizeof(b_heap_tuple_t));
	size_t size = b_heap_size(heap),
		   i = 0;

	while ( ! b_heap_empty(heap) ) {
		temp[i++] = b_heap_front_tuple(heap);
		b_heap_pop(heap);
	}

	/** Reinsert so they will be in order */
	for ( i = 0; i < size; ++i )
		b_heap_insert(heap, temp[i].value, temp[i].priority);

	if ( temp != NULL ) // Could happen if size == 0
		free(temp);
}

/** Checks if element at the given index meets the condition with their childs */
int b_heap_has_valid_direct_desdendants(b_heap_t* heap, size_t index) {
	size_t left_index,
		   right_index;

	b_heap_priority_t priority = b_heap_get_priority(heap, index);
	left_index = B_HEAP_LEFT_INDEX(index);
	right_index = left_index + 1;

	/** If we have left index and it's invalid... */
	if ( left_index < heap->size )
		if ( priority > b_heap_get_priority(heap, left_index)  )
			return 0;

	/** The same with right */
	if ( right_index < heap->size )
		if ( priority > b_heap_get_priority(heap, right_index) )
			return 0;

	return 1;
}

/** Check if heap is not ill formed */
int b_heap_is_valid(b_heap_t* heap) {
	size_t i;

	for ( i = 0; i < heap->size; ++i )
		if ( ! b_heap_has_valid_direct_desdendants(heap, i) )
			return 0;

	return 1;
}
