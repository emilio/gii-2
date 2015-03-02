#ifndef BINARY_HEAP_H_
#define BINARY_HEAP_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef BINARY_HEAP_VALUE_TYPE
#define BINARY_HEAP_VALUE_TYPE int
#endif

#define BINARY_HEAP_INITIAL_CAPACITY 8

typedef BINARY_HEAP_VALUE_TYPE b_heap_value_t;
typedef size_t b_heap_priority_t;

#define B_HEAP_PARENT_INDEX(i) ((i + 1) / 2)
#define B_HEAP_LEFT_INDEX(i) ((i + 1) * 2 - 1)
#define B_HEAP_RIGHT_INDEX ((i + 1) * 2)

typedef struct b_heap_tuple {
	b_heap_value_t value;
	b_heap_priority_t priority;
} b_heap_tuple_t;

typedef struct b_heap {
	b_heap_tuple_t* elements;
	size_t size;
	size_t capacity;
} b_heap_t;

/** Construct a new binary heap */
b_heap_t* b_heap_new();

void b_heap_destroy(b_heap_t*);

/** Insert a new element in the binary tree. -1 on error. */
void b_heap_insert(b_heap_t*, b_heap_value_t, b_heap_priority_t);

/** Get the higher priority element (which is the head, aka the field with lower priority) */
b_heap_value_t b_heap_front(b_heap_t*);

/** Delete the higher priority element */
void b_heap_pop(b_heap_t*);

/** Reposition heap node at given index looking before it */
void b_heap_heapify_to(b_heap_t* heap, size_t);

/** Reposition heap node at given index looking after it */
void b_heap_heapify_from(b_heap_t*, size_t);

/** Ensure all the heap is sorted acordingly */
void b_heap_heapify(b_heap_t*);

#endif
