/**
 * Reconstruct a binary tree given it's preorder and order elements
 *
 * NOTE:
 *   This may not work if some element is repeated, next step
 *   is to provide a solution via testing every possible option
 *   (while index_of... != NPOS)
 *
 * @author Emilio Cobos <emiliocobos@usal.es>
 * @license GPLv3 or greater
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BINARY_TREE_VALUE_TYPE char
#define INDENT_GAP 4
#include "lib/binary-tree.h"

#define NPOS ((size_t) -1)

#define FATAL_ERROR(msg, ...) do { \
	fprintf(stderr, "error: "msg, ## __VA_ARGS__); \
	exit(1); \
} while (0)


size_t index_of(const b_tree_value_t* arr, size_t length, b_tree_value_t val, size_t from_index) {
	size_t i;

	if ( length < from_index )
		return NPOS;

	arr += from_index;
	length -= from_index;

	for ( i = 0; i < length; ++i )
		if ( arr[i] == val )
			return i + from_index;

	return NPOS;
}

/**
 * This is the easy version of the function
 * It's not used in this program, just for reference
 */
b_tree_t* b_tree_reconstruct(const b_tree_value_t* preorder, const b_tree_value_t* order, size_t length) {
	size_t parent_index_order;
	b_tree_t* ret;

	/** Find the center of the order tree, which is also the number of elements in the left of the tree */
	/** Note that if length == 0 NPOS will be returned an we'll return null */
	parent_index_order = index_of(order, length, preorder[0], 0);

	/** Something bad happened: the parent value is not here */
	if ( parent_index_order == NPOS )
		return NULL;

	/** The first value in preorder is the root of the tree */
	ret = b_tree_new(preorder[0]);

	/** Pick the left elements */
	ret->left = b_tree_reconstruct(preorder + 1, order, parent_index_order);

	/** And the right ones */
	ret->right = b_tree_reconstruct(preorder + 1 + parent_index_order, order + 1 + parent_index_order, length - parent_index_order - 1);

	/** If should have leaves but it doesn't... return null, it's invalid */
	if ( (parent_index_order != 0 && ret->left == NULL) ||
		 (parent_index_order != length - 1 && ret->right == NULL) ) {
		b_tree_destroy(ret);

		return NULL;
	}

	return ret;
}

/** This is the cool version: This tries to find a version, and keeps looking if there are no matches */
b_tree_t* b_tree_reconstruct_bulletproof(const b_tree_value_t* preorder, const b_tree_value_t* order, size_t length) {
	size_t parent_index_order;
	size_t search_from = 0;
	b_tree_t* ret;

	if ( length == 0 )
		return NULL;

	/** The first value in preorder is the root of the tree */
	ret = b_tree_new(preorder[0]);

	/** To allow repeated values we must while it */
	while ( 1 ) {
		/** Try to find the center of the order tree, which is also the number of elements in the left of the tree */
		parent_index_order = index_of(order, length, preorder[0], search_from);

		/** Something bad happened: we use this null below if we are a "child" function
		  or return it if theres no other possible combination */
		if ( parent_index_order == NPOS ) {
			b_tree_destroy(ret);
			return NULL;
		}

		/** Pick the left elements */
		ret->left = b_tree_reconstruct_bulletproof(preorder + 1, order, parent_index_order);

		/** And the right ones */
		ret->right = b_tree_reconstruct_bulletproof(preorder + 1 + parent_index_order, order + 1 + parent_index_order, length - parent_index_order - 1);

		/** If should have values but it hasn't... try with the next match  */
		if ( (parent_index_order != 0 && ret->left == NULL) ||
			 (parent_index_order != length - 1 && ret->right == NULL) ) {
			search_from = parent_index_order + 1;

			if ( ret->right ) {
				b_tree_destroy(ret->right);
				ret->right = NULL;
			}

			if ( ret->left ) {
				b_tree_destroy(ret->left);
				ret->left = NULL;
			}

			continue;
		}

		/** Else we go out */
		break;
	}

	return ret;
}

void print_element(b_tree_node_t* node, void* data) {
	printf("%c\t", node->value);
}

void debug_tree(b_tree_t* head, int depth) {
    if ( ! head ) {
        printf("%*c<nil>\n", depth * INDENT_GAP, ' ');
        return;
    }

    printf("%*c%c\n", depth * INDENT_GAP, ' ', head->value);
    debug_tree(head->left, depth + 1);
    debug_tree(head->right, depth + 1);
}

void print_usage(int argc, char** argv) {
	printf("Usage:\n");
	printf("\t%s <preorder> <order>\n", argv[0]);
	printf("Example:\n");
	printf("\t%s 1234 3421\n", argv[0]);
	printf("Notes:\n");
	printf("\tThis program may or may not work with repeated values, be careful\n");
	exit(1);
}

/** Usage: ./reconstruct <preorder> <order> */
int main(int argc, char** argv) {
	b_tree_t* tree;
	size_t length;

	if ( argc != 3 )
		print_usage(argc, argv);

	length = strlen(argv[1]);

	if ( strlen(argv[2]) != length )
		FATAL_ERROR("Input with different lengths (%zu, %zu)\n", length, strlen(argv[2]));

   	tree = b_tree_reconstruct_bulletproof(argv[1], argv[2], length);

	if ( tree == NULL )
		FATAL_ERROR("Input is not convertible to tree\n");

	printf("Conversion succeeded! you should see here your input:\n");

	b_tree_preorder(tree, print_element, NULL);
	printf("\n");

	b_tree_order(tree, print_element, NULL);
	printf("\n");

    printf("Tree debugging:\n");
    debug_tree(tree, 0);
    printf("\n");

	b_tree_destroy(tree);

	return 0;
}
