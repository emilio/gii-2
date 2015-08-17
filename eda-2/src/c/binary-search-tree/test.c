#include <stdio.h>
#include <stdlib.h>

#include "lib/binary-search-tree.h"

int main() {
    bst_t* tree = bst_new();
    int i;

    for (i = 1; i < 10; ++i)
        bst_insert(tree, i);
    bst_debug(tree);

    for (i = 1; i < 10; ++i)
        bst_insert(tree, -i);
    bst_debug(tree);

    return 0;
}
