#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lib/disjoint-sets.h"

#define TEST_SIZE 1000

int main() {
    disjoint_sets_t* sets = disjoint_sets_new_with_size(TEST_SIZE);

    /** Ensure correct initialization */
    assert(sets->sets[0] == 0);
    assert(sets->sets[TEST_SIZE - 1] == TEST_SIZE - 1);

    disjoint_sets_join(sets, 0, 1);
    disjoint_sets_join(sets, 2, 0);

    assert(disjoint_sets_are_same_set(sets, 2, 1));

    disjoint_sets_add_set(sets);
    assert(sets->size == TEST_SIZE + 1);

    disjoint_sets_join(sets, 0, TEST_SIZE);
    assert(disjoint_sets_are_same_set(sets, 2, TEST_SIZE));

    disjoint_sets_destroy(sets);

    printf("All correct! :P\n");

    return 0;
}
