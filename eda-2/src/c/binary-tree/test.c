#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lib/binary-tree.h"

int main(int argc, char** argv) {
    b_tree_node_t* a = b_tree_node_new('A');
    a->left = b_tree_node_new('B');
    a->right = b_tree_node_new('C');
    a->left->left = b_tree_node_new('D');
    a->left->right = b_tree_node_new('E');
    a->right->left = b_tree_node_new('F');
    a->right->right = b_tree_node_new('G');

    printf("b_tree_node_t<%p>: count=%zu, height=%zu\n", a,
           b_tree_node_count(a), b_tree_node_height(a));

    assert(b_tree_node_count(a) == 7);

    return 0;
}
