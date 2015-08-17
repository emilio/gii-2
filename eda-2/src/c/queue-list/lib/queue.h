/**
 * Dynamically growing linked-list queue
 *
 * @author Emilio Cobos <emiliocobos@usal.es>
 * @license GPLv3 or greater
 */
#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef QUEUE_VALUE_TYPE
#define QUEUE_VALUE_TYPE int
#endif

typedef QUEUE_VALUE_TYPE queue_value_t;

typedef struct queue_item {
    queue_value_t value;
    struct queue_item* next;
} queue_item_t;

typedef struct queue {
    queue_item_t* first;
    queue_item_t* last;
    size_t size;
} queue_t;

queue_t* queue_new();

#define queue_empty(queue) (queue->size == 0)

queue_value_t queue_pop(queue_t* queue);

void queue_push(queue_t* queue, queue_value_t val);

#define queue_front(queue) (queue->first->value)

void queue_destroy(queue_t* queue);

#endif
