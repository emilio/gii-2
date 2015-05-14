#ifndef BST_H_
#define BST_H_

#define BST_IS_LEAF(node) (! (node->left && node->right) )

typedef int bst_value_t;

typedef struct bst_node {
    struct bst_node* left;
    struct bst_node* right;
    short balance_factor;
    bst_value_t value;
} bst_node_t;

typedef struct bst {
    bst_node_t* head;
} bst_t;

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
}

// Rotate a node to the right
// TODO: Review carefully
void bst_node_rotate_right(bst_ndoe_t* self) {
    bst_value_t temp_value;

    assert(self->left);

    // We swap by value, and move childrens to the right
    temp_value = self->value;
    self->value = self->left->value;
    self->left->value = temp_value;

    self->right = self->left;
    self->left = self->left->left;
    self->right->left = self->left->right;
}

// The same thing, but to the left
void bst_node_rotate_left(bst_node_t*self) {
    bst_value_t temp_value;

    temp_value = self->value;
    self->value = self->right->value;
    self->right->value = temp_value;

    self->left = self->right;
    self->right = self->right->right;
    self->left->right = self->right->left;
}

// We lost balance, reestructure our children
void bst_node_reestructure(bst_node_t* self) {
    bst_node_t* temp_node;
    bst_value_t temp;
    if ( self->balance_factor == -2 ) { // The left child has increased
        if ( self->left->balance_factor == -1 ) { // left-left -> rotate_right()
            bst_node_rotate_right(self);
        }

    }

}

void bst_node_insert(bst_node_t* self, unsigned char* height_changed, bst_node_t* node) {
    bst_node_t* parent = self;
    // We must insert it in the left
    // of the tree
    if ( self->value < val ) {
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
                self->balance_factor--;
                if ( self->balance_factor == 0) // Our height has not changed
                    *height_changed = 0;
                else if ( self->balance_factor == -2 ) // Our height has changed, and we have lost balance
                    bst_node_reestructure(self);
                // else our height changed, but we haven't lost balance
                // so no need to touch
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
                self->balance_factor++;
                if ( self->balance_factor == 0 )
                    *height_changed = 0;
                else if ( self->balance_factor == 2 )
                    bst_node_reestructure(self);
            }
        }
    }
}

#endif
