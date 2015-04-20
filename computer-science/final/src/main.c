#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#define INDENT "  "

int yylex();
int yyparse();

typedef struct node {
    struct node* prev;
    struct node* next;
    int element_count;
    struct node** elements;
    char* type;
} node_t;

/// The global execution tree
node_t* nodes = NULL;
int node_count = 0;

char* strdup(const char* const str) {
    size_t len = strlen(str);
    char* ret = (char*) malloc(len + 1);
    strncpy(ret, str, len);
    ret[len + 1] = '\0';

    return ret;
}

node_t* node_new(const char* const type, int count, ...) {
    node_t* ret = (node_t*) malloc(sizeof(node_t) + count * sizeof(node_t*));
    int i = 0;
    va_list args;

    assert(ret);

    if ( nodes )
        nodes->prev = ret;

    ret->next = nodes;
    nodes = ret;

    ret->next = NULL;
    ret->prev = NULL;
    ret->type = strdup(type);

    ret->element_count = count;
    ret->elements = count == 0 ? NULL : (node_t**) (ret + 1);

    va_start(args, count);

    while ( i < count )
        ret->elements[i++] = va_arg(args, node_t*);

    va_end(args);

    ++node_count;

    return ret;
}

node_t* atom_new(const char* const val) {
    return node_new(val, 0);
}

node_t* node_nil() {
    return atom_new("<nil>");
}

node_t* node_append(node_t* node, int count, ...) {
    va_list args;
    int i;
    node = (node_t*) realloc(node, sizeof(node_t) + (node->element_count + count) * sizeof(node_t*));

    /// Remove the node from the list
    if ( node->prev )
        node->prev->next = node->next;

    if ( node->next )
        node->next->prev = node->prev;

    va_start(args, count);

    i = 0;
    while ( i < count ) {
        node->elements[node->element_count++] = va_arg(args, node_t*);
        ++i;
    }

    va_end(args);

    return node;
}

void indent(int depth) {
    while ( depth-- )
        printf("%s", INDENT);
}

void print_node(node_t* node, int depth) {
    int i;
    indent(depth);

    if ( node->element_count == 0 ) {
        printf("Atom<%s>\n", node->type);
        return;
    }

    printf("%s[%d]\n", node->type, node->element_count);

    for ( i = 0; i < node->element_count; ++i )
        print_node(node->elements[i], depth + 1);

    if ( node->next )
        print_node(node->next, depth);
}

int yyerror(const char* const msg) {
    fprintf(stderr, "%s\n", msg);
    return 1;
}

int main(int argc, char** argv) {
    yyparse();

    print_node(nodes, 0);

    // TODO: Free nodes

    return 0;
}
