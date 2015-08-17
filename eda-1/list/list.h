#ifndef __LIST_H
#define __LIST_H
#include <stdio.h>
#include <stdlib.h>

typedef int value_t;

typedef struct item_t {
    value_t value;
    struct item_t* next;
} item_t;

typedef struct list_t {
    item_t* fake;
    item_t* end;
    size_t size;
} list_t;

typedef item_t* iterator_t;

/**
 * Allocate a new list and return it
 * On error null is returned and list_errno is set appropiatelly
 */
list_t* list_alloc();

/** Frees all the nodes and destroys the list */
void list_destroy(list_t*);

/** Initialize list. On error -1 is returned and list_errno is set appropiatelly
 */
int list_init(list_t*);

/** Know if the list is empty */
// char empty(list_t *);
#define empty(list) (list->fake == list->end)

#define size(list) (list->size)

/** Get the first element of the list */
// iterator_t begin(list_t *);
#define begin(list) (list->fake)

/** Get the next element of the list */
// iterator_t next(iterator_t);
#define next(it) (it->next)

/** Get the previous element of the list, this needs the list */
iterator_t prev(list_t*, iterator_t);

/** Get the last iterator (maybe caching it will be smart) */
#define last(list) (prev(list, end(list)))

/** Get the last element of the list plus one! perfect for iterators */
// iterator_t end(list_t *);
#define end(list) (list->end)

/** Get the position of the element of value = value_t. returns end() if not
 * found */
iterator_t find(list_t*, value_t);

/** Get the value associated with the current position, which is really the
 * value of the next node */
// value_t get(iterator_t);
#define get(it) (it->next->value)

/** Insert an element */
iterator_t insert(list_t*, iterator_t, value_t);

/** Insert an element at the end */
// void push(list_t *, value_t);
#define push(list, value) (insert(list, end(list), value))

/** Insert an element at the beginning */
// void unshift(list_t *, value_t);
#define unshift(list, value) (insert(begin(list), value))

/** Delete an element. returns the deleted value */
value_t delete (list_t*, iterator_t);

/** Delete the first element */
// value_t pop(list_t *);
#define pop(list) (delete (begin(list)))

/** Delete the last element */
// value_t shift(list_t *);
#define shift(list) (delete (list, last(list)))

/** Prints the list */
void print(list_t*);
#endif
