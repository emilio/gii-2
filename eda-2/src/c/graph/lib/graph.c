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

	ret->reached = 0;
    ret->distance = GRAPH_INFINITE_DISTANCE;
    ret->reached_from = 0;
	ret->incoming_edges_count = 0;
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

void graph_recompute(graph_t* self, int flags) {
	size_t i, j;
	if ( flags == 0 )
		return;

	if ( flags & GRAPH_RECOMPUTE_REACHED_BIT
		 || flags & GRAPH_RECOMPUTE_INCOMING_BIT ) {
		for ( i = 0; i < self->size; ++i ) {
			if ( flags & GRAPH_RECOMPUTE_REACHED_BIT ) {
				self->v[i]->reached = 0;
                self->v[i]->reached_from = 0;
            }

            if ( flags & GRAPH_RECOMPUTE_DISTANCE_BIT )
                self->v[i]->distance = GRAPH_INFINITE_DISTANCE;

			if ( flags & GRAPH_RECOMPUTE_INCOMING_BIT )
				self->v[i]->incoming_edges_count = 0;
		}
	}

	if ( flags & GRAPH_RECOMPUTE_INCOMING_BIT ) {
		for ( i = 0; i < self->size; ++i ) {
			adjacent_t* current = self->v[i]->adjacents_head;
			while ( current != NULL ) {
				self->v[current->id]->incoming_edges_count += 1;
				current = current->next;
			}
		}
	}
}

vertex_t** graph_topological_sort(graph_t* self) {
	vertex_t** ret;
    vertex_t* initial_vertex = NULL;
    size_t i;

	graph_recompute(self, GRAPH_RECOMPUTE_REACHED_BIT | GRAPH_RECOMPUTE_INCOMING_BIT);

    for ( i = 0; i < self->size; ++i ) {
        if ( self->v[i]->incoming_edges_count == 0 ) {
            initial_vertex = self->v[i];
            break;
        }
    }

    if ( ! initial_vertex )
        return NULL;

    ret = (vertex_t**) malloc(self->size * sizeof(vertex_t**));

	if ( ! ret )
		return ret;

    ret[0] = initial_vertex;

	// TODO

	return ret;
}

#endif
