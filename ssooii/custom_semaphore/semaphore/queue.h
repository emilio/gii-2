#ifndef QUEUE_H
#define QUEUE_H

#include <unistd.h>
#include <assert.h>

#ifndef QUEUE_MAX_ITEMS
#define QUEUE_MAX_ITEMS 255
#endif

typedef pid_t queue_value_t;

typedef struct queue {
    queue_value_t items[QUEUE_MAX_ITEMS];
    size_t capacity;
    size_t size;
    size_t first;
} queue_t;

void queue_init(queue_t* queue) {
    queue->capacity = QUEUE_MAX_ITEMS;
    queue->size = queue->first = 0;
}

#define queue_last_index(q) ((q->first + q->size) % q->capacity)
#define queue_next_index(q, i) ((i + 1) % q->capacity)

void queue_push(queue_t* queue, queue_value_t val) {
    assert(queue->size != queue->capacity);

    queue->items[queue_last_index(queue)] = val;
    queue->size++;
}

void queue_pop(queue_t* queue) {
    queue->first = queue_next_index(queue, queue->first);
    queue->size--;
}

queue_value_t queue_front(queue_t* queue) { return queue->items[queue->first]; }

#endif
