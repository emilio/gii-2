#include "lib/graph.h"

int main() {
    graph_t* graph = graph_new_with_count(5);

    printf("graph created: %p\n", graph);

    vertex_adjacent_add(graph->v[0], 2);

    printf("added adjacent from 0 to 2\n");

    vertex_id_t* ordered = graph_topological_sort(graph);

    printf("Sorted\n");

    printf("Size: %zu\n", graph->size);

    for ( size_t i = 0; i < graph->size; ++i ) {
        printf("%zu\t", ordered[i]);
    }
    printf("\n");

    free(ordered);
    graph_destroy(graph);

    return 0;
}
