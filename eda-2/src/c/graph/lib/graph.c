#include "graph.h"
#include "queue.h"
#include "binary-heap.h"

/** Create a new representation for an adjacent vertex */
adjacent_t* adjacent_new_weighted(vertex_id_t id, size_t weight) {
	adjacent_t* ret = (adjacent_t*) malloc(sizeof(adjacent_t));

	assert(ret != NULL);

    ret->weight = weight;
	ret->id = id;
	ret->next = NULL;
	return ret;
}

adjacent_t* adjacent_new(vertex_id_t id) {
    return adjacent_new_weighted(id, 0);
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
void vertex_adjacent_add_weighted(vertex_t* self, vertex_id_t id, size_t weight) {
	if ( self->adjacents_head == NULL ) {
		self->adjacents_head = self->adjacents_last = adjacent_new_weighted(id, weight);
		return;
	}

	self->adjacents_last->next = adjacent_new(id);
	self->adjacents_last = self->adjacents_last->next;
}

void vertex_adjacent_add(vertex_t* self, vertex_id_t id) {
    return vertex_adjacent_add_weighted(self, id, 0);
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

// Dijkstra's algorithm: Compute the distance from a vertex
// This algorithm uses a priority queue (ordered by min distance)
// Returns 0 on success, negative value on error
int graph_shortest_path_from(graph_t* self, vertex_id_t from) {
    b_heap_t* heap;
    vertex_id_t current_id;
    vertex_t* current_vertex;
    adjacent_t* current_adjacent;
    size_t distance = 0;

    if ( from >= self->size )
        return -1;

    heap = b_heap_new();

    if ( ! heap )
        return -1;

    graph_recompute(self, GRAPH_RECOMPUTE_REACHED_BIT | GRAPH_RECOMPUTE_DISTANCE_BIT);

    self->v[from]->distance = 0;

    // The heap initially contains just the first vertex, with a distance 0
    b_heap_insert(heap, from, 0);

    while ( ! b_heap_empty(heap) ) {
        current_id = b_heap_front(heap);
        b_heap_pop(heap);

        current_vertex = self->v[current_id];
        current_adjacent = current_vertex->adjacents_head;

        if ( current_vertex->reached )
            continue;

        current_vertex->reached = 1;

        // Check all the neighbors, calculate distance to them
        while ( current_adjacent ) {
            distance = current_vertex->distance + current_adjacent->weight;

            // If found a shorter path, add them to the queue
            // Note that at this point an element can be multiple times in
            // the queue, but since it's inserted with different priority
            // the node will be processed at the correct time
            if ( self->v[current_adjacent->id]->distance > distance ) {
                self->v[current_adjacent->id]->distance = distance;
                self->v[current_adjacent->id]->reached_from = current_id;
                b_heap_insert(heap, current_adjacent->id, distance);
            }
            current_adjacent = current_adjacent->next;
        }
    }

    b_heap_destroy(heap);

    return 0;
}
