#include "binary-tree.h"

/** Create a tree node */
b_tree_node_t* b_tree_node_new(b_tree_value_t value) {
	b_tree_node_t* ret = malloc(sizeof(b_tree_node_t));

	assert(ret != NULL);

	ret->left = ret->right = NULL;
	ret->value = value;

	return ret;
}

/** Destroy a tree node */
void b_tree_node_destroy(b_tree_node_t* node) {
	if ( node->left )
		b_tree_node_destroy(node->left);

	if ( node->right )
		b_tree_node_destroy(node->right);

	free(node);
}

/** Get the height of the tree */
size_t b_tree_node_height(b_tree_node_t* node) {
	size_t left, right;

	if ( node == NULL )
		return 0;

	left = b_tree_node_height(node->left);
	right = b_tree_node_height(node->right);

	return 1 + (left > right ? left : right);
}

/** Transverse the tree in pre-order */
void b_tree_node_preorder(b_tree_node_t* node, b_tree_callback_t cb, void* data) {

	cb(node, data);

	if ( node->left )
		b_tree_node_preorder(node->left, cb, data);

	if ( node->right )
		b_tree_node_preorder(node->right, cb, data);
}

/** Transverse the tree in order */
void b_tree_node_order(b_tree_node_t* node, b_tree_callback_t cb, void* data) {
	if ( node->left )
		b_tree_node_order(node->left, cb, data);

	cb(node, data);

	if ( node->right )
		b_tree_node_order(node->right, cb, data);
}

/** Transverse the tree in post-order */
void b_tree_node_postorder(b_tree_node_t* node, b_tree_callback_t cb, void* data) {
	if ( node->left )
		b_tree_node_postorder(node->left, cb, data);

	if ( node->right )
		b_tree_node_postorder(node->right, cb, data);

	cb(node, data);
}

/** Iterate a level of given node */
void b_tree_node_level(b_tree_node_t* node, size_t level, b_tree_callback_t cb, void* data) {
	if ( level == 0 ) {
		cb(node, data);
		return;
	}

	if ( node->left )
		b_tree_node_level(node->left, level - 1, cb, data);

	if ( node->right )
		b_tree_node_level(node->right, level - 1, cb, data);
}

void b_tree_node_levelorder(b_tree_node_t* node, b_tree_callback_t cb, void* data) {
	size_t height = b_tree_node_height(node),
		   i;

	for ( i = 0; i < height; ++i )
		b_tree_node_level(node, i, cb, data);
}


/** TODO: Try to avoid duplication in _count functions */
static void __b_tree_node_count(b_tree_node_t* _, void* acum) {
	*((size_t*)acum) += 1;
}

static void __b_tree_leaf_count(b_tree_node_t* node, void* acum) {
	if ( node->left == NULL && node->right == NULL )
		*((size_t*)acum) += 1;
}

static void __b_tree_non_leaf_count(b_tree_node_t* node, void* acum) {
	if ( node->left || node->right )
		*((size_t*)acum) += 1;
}

static void __b_tree_one_child_count(b_tree_node_t* node, void* acum) {
	if ( (node->left && ! node->right) ||
	   (node->right && ! node->left) )
		*((size_t*)acum) += 1;

}

#define B_TREE_COUNT_FN(what) \
size_t b_tree_ ## what ## _count(b_tree_node_t* node) { \
	size_t result = 0; \
	b_tree_node_preorder(node, __b_tree_ ## what ## _count, (void*) &result); \
	return result; \
}

B_TREE_COUNT_FN(node)
B_TREE_COUNT_FN(leaf)
B_TREE_COUNT_FN(non_leaf)
B_TREE_COUNT_FN(one_child)


static void __b_tree_search_and_replace(b_tree_node_t* node, void* data_) {
	b_tree_search_data_t* data = (b_tree_search_data_t*) data_;

	if ( node->value == data->search ) {
		node->value = data->replace;
		data->count++;
	}
}

size_t b_tree_node_search_and_replace(b_tree_node_t* node, b_tree_value_t search, b_tree_value_t replace) {
	b_tree_search_data_t search_data;

	search_data.search = search;
	search_data.replace = replace;
	search_data.count = 0;

	b_tree_node_preorder(node, __b_tree_search_and_replace, (void*) &search_data);

	return search_data.count;
}
