#ifndef BST_H_
#define BST_H_

#define BST_DEBUG_INDENT_LEVEL 4
#define BST_IS_LEAF(node) (!(node->left && node->right))

typedef int bst_value_t;

typedef struct bst_node {
    struct bst_node* left;
    struct bst_node* right;
    short balance_factor;
    bst_value_t value;
} bst_node_t;

typedef struct bst { bst_node_t* head; } bst_t;

bst_t* bst_new();

bst_node_t* bst_node_new(bst_value_t val);

void bst_insert(bst_t* self, bst_value_t val);

void bst_debug(bst_t* self);

void bst_node_debug(bst_node_t* self, int depth);

void bst_node_rotate_right(bst_node_t* self);

// The same thing, but to the left
void bst_node_rotate_left(bst_node_t* self);

// We lost balance, reestructure our children
void bst_node_reestructure(bst_node_t* self);

void bst_node_insert(bst_node_t* self, unsigned char* height_changed,
                     bst_node_t* node);

#endif
