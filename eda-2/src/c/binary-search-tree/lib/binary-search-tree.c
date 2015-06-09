#include "binary-search-tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

bst_t* bst_new() {
    bst_t* ret = (bst_t*) malloc(sizeof(bst_t));

    assert(ret);

    ret->head = NULL;
    return ret;
}

bst_node_t* bst_node_new(bst_value_t val) {
    bst_node_t* ret = (bst_node_t*) malloc(sizeof(bst_node_t));

    assert(ret);

    ret->left = ret->right = NULL;
    ret->balance_factor = 0;
    ret->value = val;

    return ret;
}

void bst_insert(bst_t* self, bst_value_t val) {
    unsigned char changed = 0;
    bst_node_t* node = bst_node_new(val);
    if ( ! self->head )
        self->head = node;
    else
        bst_node_insert(self->head, &changed, node);
    bst_debug(self);
}

void bst_debug(bst_t* self) {
    bst_node_debug(self->head, 0);
}

void bst_node_debug(bst_node_t* self, int depth) {
    if ( self == NULL ) {
        printf("%*c<nil>\n", depth * BST_DEBUG_INDENT_LEVEL, ' ');
        return;
    }

    printf("%*c%d (bf: %hd)\n", depth * BST_DEBUG_INDENT_LEVEL, ' ', self->value, self->balance_factor);
    bst_node_debug(self->left, depth + 1);
    bst_node_debug(self->right, depth + 1);
}

// Rotate a node to the right
// TODO: Review carefully
void bst_node_rotate_right(bst_node_t* self) {
    bst_value_t temp_value;
    bst_node_t* old_right;

    assert(self->left);

    old_right = self->right;
    // We swap by value, and move childrens to the right
    temp_value = self->value;
    self->value = self->left->value;
    self->left->value = temp_value;

    self->right = self->left;
    self->left = self->left->left;

    self->right->left = self->right->right;
    self->right->right = old_right;

    self->balance_factor++;
    // right was the old left
    self->right->balance_factor++;
}

// The same thing, but to the left
void bst_node_rotate_left(bst_node_t*self) {
    bst_value_t temp_value;
    bst_node_t* old_left;

    assert(self->right);

    old_left = self->left;
    temp_value = self->value;
    self->value = self->right->value;
    self->right->value = temp_value;

    self->left = self->right;
    self->right = self->right->right;

    self->left->right = self->left->left;
    self->left->left = old_left;

    self->balance_factor--;
    // left was the old right
    self->left->balance_factor--;
}

// We lost balance, reestructure our children
void bst_node_reestructure(bst_node_t* self) {
    if ( self->balance_factor == -1 ) { // The left child has increased
        if ( self->left->balance_factor == -1 ) { // left-left -> rotate_right()
            bst_node_rotate_right(self);
        } else { // left right, simplify to left-left rotating to the left
            bst_node_rotate_left(self->left);
            bst_node_rotate_right(self);
        }
    } else if ( self->balance_factor == 1 ) { // It's the right child
        if ( self->right->balance_factor == 1 ) { // right-right -> rotate_left()
            bst_node_rotate_left(self);
        } else { // right-left, simplify to right-right rotating to the left
            bst_node_rotate_right(self->right);
            bst_node_rotate_left(self);
        }
    }
}

void bst_node_insert(bst_node_t* self, unsigned char* height_changed, bst_node_t* node) {
    // We must insert it in the left
    // of the tree
    if ( node->value < self->value ) {
        if ( ! self->left ) {
            self->left = node;
            self->balance_factor--;

            // If the height has_changed
            if ( ! self->right )
                *height_changed = 1;
        } else {
            bst_node_insert(self->left, height_changed, node);
            // The height of our left child has changed, so our balance factor
            // and posibly our height too
            if ( *height_changed ) {
                switch ( self->balance_factor ) {
                    case 1: // Our height wont change
                        self->balance_factor--;
                        *height_changed = 0;
                        break;
                    case 0: // our height will change, but it won't affect the balance factor
                        self->balance_factor--;
                        break;
                    case -1: // Our height would change, and we have lost balance
                        bst_node_reestructure(self);
                        *height_changed = 0;
                        // reestructure takes care of the balance_factor value
                        break;
                }
            }
        }
    // Else... on the right
    } else {
        if ( ! self->right ) {
            self->right = node;
            self->balance_factor++;

            if ( ! self->left )
                *height_changed = 1;
        } else {
            bst_node_insert(self->right, height_changed, node);
            if ( *height_changed ) {
                switch ( self->balance_factor ) {
                    case -1: // Our height wont change
                        self->balance_factor++;
                        *height_changed = 0;
                        break;
                    case 0: // our height will change, but it won't affect the balance factor
                        self->balance_factor++;
                        break;
                    case 1: // Our height would change, and we have lost balance
                        bst_node_reestructure(self);
                        *height_changed = 0;
                        break;
                }
            }
        }
    }
}

