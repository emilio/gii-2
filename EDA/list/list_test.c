#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"


int main(void) {
	int i;
	list_t *list = list_alloc();

	srand(time(NULL));


	for (i = 0; i < 10; i++)
		push(list, rand() % 10);
	print(list);

	insert(list, begin(list), 7);
	print(list);

	delete(list, begin(list));
	print(list);

	delete(list, last(list));
	print(list);

	list_destroy(list);

	return 0;
}
