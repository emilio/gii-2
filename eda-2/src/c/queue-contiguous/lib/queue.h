/**
 * Dynamically growing contiguous memory queue
 *
 * @author Emilio Cobos <emiliocobos@usal.es>
 * @license GPLv3.0 or later
 *
 * This is a "generic" queue implementation
 * with contiguous memory (faster access and
 * reutilization of memory).
 *
 * This queue grows if more space is needed, but is
 * aware of unused space at the left.
 * If it detects a lot of unused space at the left,
 * it moves the beginning to the left instead of
 * allocating more space.
 *
 * This implementation needs more testing than the file in test.c
 * and needs to be splitted in queue.c and queue.h files
 */
#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/** Initial allocated items on the queue */
#define QUEUE_INITIAL_SIZE 10

/** Number of items unused at the left that will force a queue move */
#define QUEUE_MAX_ITEMS_UNUSED 10

/** Base struct */
typedef struct queue {
	char* data; /** Our data, generic so we can store any type of element */
	size_t first_index; /** Index to the first element in the queue */
	size_t size; /** Number of elements used in the queue */
	size_t capacity; /** Capacity of the queue: space allocated / item_size
					      first_index + size <= capacity, always */
	size_t item_size; /** Size (in bytes) of the item we're storing */
} queue_t;

/** Know if a queue is empty */
#define queue_empty(s) (s->size == 0)

/** Move to the left or grow space in the queue */
void queue_grow(queue_t* queue) {
	size_t dest_size;

	/** Before growing we check if we have space in the left */
	if ( queue->first_index > QUEUE_MAX_ITEMS_UNUSED ) {
		/** Move everything to the front */
		memcpy(queue->data, queue->data + (queue->first_index * queue->item_size), queue->size * queue->item_size);
		queue->first_index = 0;
		return;
	}

	dest_size = queue->capacity * 2;

	if ( dest_size == 0 )
		dest_size = QUEUE_INITIAL_SIZE;

	if ( queue->data == NULL )
		queue->data = (char*) malloc(dest_size * queue->item_size);
	else
		queue->data = (char*) realloc(queue->data, dest_size * queue->item_size);

	assert(queue->data != NULL);
}


/**
 * Our "constructor" gets a type as parameter
 * that initializes item_size
 *
 * Such as: queue_new(int);
 */
#define queue_new(T) queue_new_(sizeof(T))
queue_t* queue_new_(size_t item_size) {
	queue_t* ret = (queue_t*) malloc(sizeof(queue_t));

	assert(ret != NULL);

	ret->data = NULL;
	ret->first_index = ret->size = ret->capacity = 0;
	ret->item_size = item_size;

	return ret;
}

/**
 * To push a value we must take a pointer to that value...
 *
 * That means you cant use:
 *   queue_push(my_queue, 49);
 *
 * That's the tradeoff for flexibility
 */
#define queue_push(q, val) queue_push_(q, (void*) &val)
void queue_push_(queue_t* queue, void* val) {

	assert(val != NULL);

	if ( (queue->first_index + queue->size) == queue->capacity )
		queue_grow(queue);

	memcpy(queue->data + ((queue->first_index + queue->size++) * queue->item_size), val, queue->item_size);
}


/**
 * Our pop() operation is void
 * Returning the value will add complexity.
 * It's perfectly reasonable using just front+top.
 */
void queue_pop(queue_t* queue) {
	assert(! queue_empty(queue));
	queue->first_index++;
	queue->size--;
}

/**
 * Our front function must take a pointer to store the value in.
 * That's again a tradeoff for flexibility in the queue type.
 */
void queue_front(queue_t* queue, void* val) {
	assert(val != NULL);
	assert(queue->size != 0);
	memcpy(val, queue->data + ((queue->first_index) * queue->item_size), queue->item_size);
}

/** To destroy or queue we just have to destroy the `data` field */
void queue_destroy(queue_t* queue) {
	if ( queue->data != NULL )
		free(queue->data);
	free(queue);
}

#endif
