#ifndef GRAPH_H_
#define GRAPH_H_
#include <stdlib.h>
#include <stdio.h>

#define GRAPH_RECOMPUTE_REACHED_BIT   0x01
#define GRAPH_RECOMPUTE_INCOMING_BIT  0x02
#define GRAPH_RECOMPUTE_DISTANCE_BIT  0x04

#define GRAPH_RECOMPUTE_ALL           0xFF
#define GRAPH_INFINITE_DISTANCE ((size_t) -1)

typedef size_t vertex_id_t;

typedef struct adjacent {
	vertex_id_t id;
    size_t weight;
    size_t distance;
	struct adjacent* next;
} adjacent_t;

typedef struct vertex {
	unsigned char reached;
    vertex_id_t reached_from;
    size_t distance;
	size_t incoming_edges_count;
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
vertex_t* vertex_new_weighed(size_t weight);

void vertex_adjacent_add(vertex_t* self, vertex_id_t id);

graph_t* graph_new_with_count(size_t vertex_count);

graph_t* graph_new();

void graph_recompute(graph_t*, int flags);

vertex_t** graph_topological_sort(graph_t*);

#endif
