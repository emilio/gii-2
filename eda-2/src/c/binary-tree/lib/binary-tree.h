#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

#include <stdlib.h>
#include <assert.h>

#ifndef BINARY_TREE_VALUE_TYPE
#define BINARY_TREE_VALUE_TYPE int
#endif

typedef BINARY_TREE_VALUE_TYPE b_tree_value_t;

/** Node structure */
typedef struct b_tree_node {
	struct b_tree_node* left;
	struct b_tree_node* right;
	b_tree_value_t value;
} b_tree_node_t;

/** It's about semantics, and can be redefined */
typedef b_tree_node_t b_tree_t;

typedef void (*b_tree_callback_t)(b_tree_node_t*, void*);

#define b_tree_new b_tree_node_new
#define b_tree_destroy b_tree_node_destroy

/** Create/Destroy a node */
b_tree_node_t* b_tree_node_new(b_tree_value_t);
void b_tree_node_destroy(b_tree_node_t*);

#define b_tree_height b_tree_node_height
size_t b_tree_node_height(b_tree_node_t*);

/** Total node count */
size_t b_tree_node_count(b_tree_node_t*);

/** Leaf count */
size_t b_tree_leaf_count(b_tree_node_t*);

/** Non-leaf count */
size_t b_tree_non_leaf_count(b_tree_node_t*);

/** Nodes with one child count */
size_t b_tree_one_child_count(b_tree_node_t*);


typedef struct b_tree_search_data {
	b_tree_value_t search;
	b_tree_value_t replace;
	size_t count;
} b_tree_search_data_t;

/** Search and replace a value, returning the number of replaces made */
size_t b_tree_node_search_and_replace(b_tree_node_t*, b_tree_value_t,	b_tree_value_t);

/** Preorder loop: void* -> custom data */
#define b_tree_preorder b_tree_node_preorder
void b_tree_node_preorder(b_tree_node_t*, b_tree_callback_t, void*);

/** Postorder loop */
#define b_tree_postorder b_tree_node_postorder
void b_tree_node_postorder(b_tree_node_t*, b_tree_callback_t, void*);

/** Order loop */
#define b_tree_order b_tree_node_order
void b_tree_node_order(b_tree_node_t*, b_tree_callback_t, void*);

/** Iterate through given level */
#define b_tree_level b_tree_node_level
void b_tree_node_level(b_tree_node_t*, size_t, b_tree_callback_t, void*);

/** Iterate in level order */
#define b_tree_levelorder b_tree_node_levelorder
void b_tree_node_levelorder(b_tree_node_t*, b_tree_callback_t, void*);


#endif
