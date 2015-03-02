#include <stdlib.h>
#include <stdio.h>

#include "lib/binary-heap.h"

void print_b_heap(b_heap_t* heap) {
	size_t i;
	for ( i = 0; i < heap->size; ++i )
		printf("{%d, %zu}\t", heap->elements[i].value, heap->elements[i].priority);
	printf("\n");

}
int main(int argc, char** argv) {
	b_heap_t* heap = b_heap_new();

	b_heap_insert(heap, 2, 0); // Max priority
	b_heap_insert(heap, 5, 4);
	b_heap_insert(heap, 6, 2);
	b_heap_insert(heap, 4, 1);

	print_b_heap(heap);

	b_heap_pop(heap);

	print_b_heap(heap);

	heap->elements[2].priority = 0;

	print_b_heap(heap);

	b_heap_heapify(heap);

	print_b_heap(heap);

	b_heap_destroy(heap);
	return 0;
}
