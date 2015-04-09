#ifndef GRAPH_H_
#define GRAPH_H_
#include <stdlib.h>
#include <stdio.h>

typedef size_t vertex_id_t;

typedef struct adjacent {
	vertex_id_t id;
	struct adjacent* next;
} adjacent_t;

typedef struct vertex {
	unsigned char visited;
	adjacent_t* adjacents_head;
	adjacent_t* adjacents_last;
} vertex_t;

typedef struct graph {
	vertex_t** v;
	size_t capacity;
	size_t size;
} graph_t;

adjacent_t* adjacent_new(vertex_id_t id);

vertex_t* vertex_new();

void vertex_adjacent_add(vertex_t* self, vertex_id_t id);

graph_t* graph_new_with_count(size_t vertex_count);

graph_t* graph_new();

#endif
