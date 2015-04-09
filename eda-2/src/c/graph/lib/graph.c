#include "graph.h"

/** Create a new representation for an adjacent vertex */
adjacent_t* adjacent_new(vertex_id_t id) {
	adjacent_t* ret = (adjacent_t*) malloc(sizeof(adjacent_t));

	assert(ret != NULL);

	ret->id = id;
	ret->next = NULL;
	return ret;
}

/** Create a new vertex */
vertex_t* vertex_new() {
	vertex_t* ret = (vertex_t*) malloc(sizeof(vertex_t));

	assert(ret != NULL);

	ret->visited = 0;
	ret->adjacents_head = ret->adjacents_last = NULL;

	return ret;
}

/** Add a new adjacent vertex to given vertex */
void vertex_adjacent_add(vertex_t* self, vertex_id_t id) {
	if ( self->adjacents_head == NULL ) {
		self->adjacents_head = self->adjacents_last = adjacent_new(id);
		return;
	}

	self->adjacents_last->next = adjacent_new(id);
	self->adjacents_last = self->adjacents_last->next;
}

/** Create a new graph with given vertex count */
graph_t* graph_new_with_count(size_t vertex_count) {
	graph_t* ret = (graph_t*) malloc(sizeof(graph_t));
	size_t i;

	assert(ret != NULL);

	if ( vertex_count == 0 )
		ret->vertices = NULL;
	else
		ret->vertices = (vertex_t**) malloc(sizeof(vertex_t**) * vertex_count);

	for ( i = 0; i < vertex_count; ++i ) {
		ret->vertices[i] = vertex_new();
	}

	ret->size = ret->capacity = vertex_count;
	return ret;
}

/** Create a new graph with no vertexes */
graph_t* graph_new() {
	return graph_new_with_count(0);
}

#endif
