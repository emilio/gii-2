#include "list.h"

/** Create a list */
list_t * list_alloc() {
	list_t *ret = (list_t *) malloc(sizeof(list_t));


	if ( ret == NULL )
		return NULL;

	if ( list_init(ret) == -1 ) {
		free(ret);
		return NULL;
	}

	return ret;
}

/** Prepare an empty list */
int list_init(list_t *list) {
	list->fake = (item_t *) malloc(sizeof(item_t));

	if ( list->fake == NULL )
		return -1;

	list->fake->next = NULL;

	list->size = 0;

	list->end = list->fake;

	return 0;
}

/** Destroy the list */
void list_destroy(list_t *list) {
	iterator_t it = begin(list);

	while ( it != end(list) )
		delete(list, it);

	free(list->fake);

	free(list);
}

/** Get the previous value */
iterator_t prev(list_t *list, iterator_t iter) {
	iterator_t it = begin(list);
	iterator_t temp;

	if ( iter == it )
		return NULL;

	while ( it != end(list) ) {
		temp = next(it);

		if ( temp == iter )
			return it;

		it = temp;
	}

	return it;
}

/** Find a value in the list */
iterator_t find(list_t *list, value_t val) {
	iterator_t it;

	for ( it = begin(list); it != end(list); it = next(it) )
		if ( get(it) == val )
			return it;

	return it;
}

/** Insert a value in the list at pos = pos */
iterator_t insert(list_t *list, iterator_t pos, value_t val) {
	item_t *node = (item_t *) malloc(sizeof(item_t));

	if ( node == NULL ) {
		list_errno = LIST_ERR_BAD_ALLOC;
		return NULL;
	}

	node->value = val;

	node->next = next(pos);

	pos->next = node;

	if ( pos == end(list) )
		list->end = node;

	++list->size;

	return pos;
}

/** Delete an element */
value_t delete(list_t *list, iterator_t pos) {
	value_t val = get(pos);
	iterator_t to_delete = next(pos);

	pos->next = next(to_delete);

	if ( to_delete == end(list) )
		list->end = pos;

	free(to_delete);

	--list->size;

	return val;
}

/** Print a list */
void print(list_t *list) {
	iterator_t it;

	printf("list[%zu] {", size(list));

	for ( it = begin(list); it != end(list); it = next(it) )
		printf("%d, ", get(it));

	printf("(end) }\n");
}
