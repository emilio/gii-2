#include <stdio.h>
#include <stdlib.h>
#include "lib/graph.h"

void graph_debug_path(graph_t* graph, vertex_id_t initial, vertex_id_t vertex_id) {
    vertex_id_t reached_from;
    if ( vertex_id == initial )
        return;

    reached_from = graph->v[vertex_id]->reached_from;


    printf("Vertex %zu was reached from %zu (distance: %zu)\n", vertex_id, reached_from, graph->v[vertex_id]->distance - graph->v[reached_from]->distance);

    graph_debug_path(graph, initial, reached_from);
}

void graph_debug_path_info(graph_t* graph, vertex_id_t initial_vertex_id) {
    size_t i;
    for ( i = 0; i < graph->size; ++i ) {
        if ( i == initial_vertex_id ) {
            printf("Vertex %zu is the initial vertex, so it has a distance of %zu\n", i, graph->v[i]->distance);
        } else if ( ! graph->v[i]->reached ) {
            printf("Vertex %zu cannot be reached from %zu\n", i, initial_vertex_id);
        } else {
            printf("Distance from %zu to %zu is %zu\n", i, initial_vertex_id, graph->v[i]->distance);
            graph_debug_path(graph, initial_vertex_id, i);
        }
    }
}

int main() {
    graph_t* graph = graph_new_with_count(5);

    printf("graph created: %p\n", graph);

    vertex_adjacent_add_weighted(graph->v[0], 2, 3);

    printf("added adjacent from 0 to 2\n");

    vertex_id_t* ordered = graph_topological_sort(graph);

    printf("Sorted\n");

    printf("Size: %zu\n", graph->size);

    for ( size_t i = 0; i < graph->size; ++i )
        printf("%zu\t", ordered[i]);

    printf("\n");

    free(ordered);

    if ( graph_shortest_path_from(graph, 0) == 0 )
        graph_debug_path_info(graph, 0);
    else
        printf("Shortest path not calculated... :/\n");

    graph_destroy(graph);

    return 0;
}
