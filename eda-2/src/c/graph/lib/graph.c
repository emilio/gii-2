#include "graph.h"
#include "../../queue-contiguous/lib/queue.h"

/** Create a new representation for an adjacent vertex */
adjacent_t* adjacent_new(vertex_id_t id) {
	adjacent_t* ret = (adjacent_t*) malloc(sizeof(adjacent_t));

	assert(ret != NULL);

	ret->id = id;
	ret->next = NULL;
	return ret;
}

void adjacent_destroy(adjacent_t* self) {
    free(self);
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

void vertex_destroy(vertex_t* self) {
    adjacent_t* current = self->adjacents_head;
    adjacent_t* next;

    while ( current ) {
        next = current->next;
        adjacent_destroy(current);
        current = next;
    }

    free(self);
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
		ret->v = NULL;
	else
		ret->v = (vertex_t**) malloc(sizeof(vertex_t**) * vertex_count);

	for ( i = 0; i < vertex_count; ++i )
		ret->v[i] = vertex_new();

	ret->size = ret->capacity = vertex_count;
	return ret;
}

/** Create a new graph with no vertexes */
graph_t* graph_new() {
	return graph_new_with_count(0);
}

void graph_destroy(graph_t* self) {
    size_t i;

    for ( i = 0; i < self->size; ++i )
        vertex_destroy(self->v[i]);

    free(self->v);
    free(self);
}

void graph_recompute(graph_t* self, int flags) {
	size_t i;
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

vertex_id_t* graph_topological_sort(graph_t* self) {
	vertex_id_t* ret;
    size_t current_id;
    vertex_t* current = NULL;
    adjacent_t* current_adjacent = NULL;
    size_t i;
    queue_t* q;

	graph_recompute(self, GRAPH_RECOMPUTE_REACHED_BIT | GRAPH_RECOMPUTE_INCOMING_BIT);

    q = queue_new(vertex_id_t);

    for ( i = 0; i < self->size; ++i )
        if ( self->v[i]->incoming_edges_count == 0 )
            queue_push(q, i);

    if ( queue_empty(q) ) {
        queue_destroy(q);
        return NULL;
    }

    ret = (vertex_id_t*) malloc(self->size * sizeof(vertex_id_t));

	if ( ! ret ) {
        queue_destroy(q);
		return ret;
    }

    i = 0;

    while ( ! queue_empty(q) ) {
        queue_front(q, &current_id);
        queue_pop(q);

        current = self->v[current_id];

        ret[i++] = current_id;

        current_adjacent = current->adjacents_head;

        while ( current_adjacent ) {

            if ( --(self->v[current_adjacent->id]->incoming_edges_count) == 0 )
                queue_push(q, current_adjacent->id);

            current_adjacent = current_adjacent->next;
        }
    }

    queue_destroy(q);

    assert(i == self->size);

	return ret;
}
