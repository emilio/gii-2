#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DEFAULT_MAX_LENGTH 10
#define SET_INITIAL_CAPACITY 10

typedef char* set_element_t;
typedef struct set {
    set_element_t* elements;
    size_t size;
    size_t capacity;
    size_t max_element_length;
} set_t;

set_t* set_new() {
    set_t* ret = (set_t*)malloc(sizeof(set_t));

    ret->elements = NULL;
    ret->size = ret->capacity = ret->max_element_length = 0;

    return ret;
}

void set_grow(set_t* set) {
    set->capacity =
        set->capacity == 0 ? SET_INITIAL_CAPACITY : set->capacity * 2;

    if (set->elements == NULL)
        set->elements =
            (set_element_t*)malloc(set->capacity * sizeof(set_element_t));
    else
        set->elements =
            realloc(set->elements, set->capacity * sizeof(set_element_t));
}

void set_add(set_t* set, set_element_t val) {
    if (set->size == set->capacity)
        set_grow(set);

    set->elements[set->size++] = val;
}

static const char* chars =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'\"";

char* str_clone(const char* str, size_t size) {
    char* ret = (char*)malloc(size + 1);
    memcpy(ret, str, size + 1);
    return ret;
}

char* str_concat(char* s1, char* s2, size_t max) {
    char* ret = (char*)malloc(max);
    *ret = '\0';
    strcat(ret, s1);
    strcat(ret, s2);

    return ret;
}

void set_destroy(set_t* set) {
    if (set->elements)
        free(set->elements);
    free(set);
}

/** Get the cartesian product of the set given times */
set_t* set_product(set_t* set_1, set_t* set_2) {
    size_t i, j;
    set_t* ret = set_new();

    for (i = 0; i < set_1->size; ++i)
        for (j = 0; j < set_2->size; ++j)
            set_add(ret, str_concat(set_1->elements[i], set_2->elements[j],
                                    ret->max_element_length));

    return ret;
}

/** Get the initial `chars` set as strings */
set_t* get_initial_set() {
    set_t* set = set_new();
    size_t length = strlen(chars); // Don't include \0 (we print each set)
    size_t i;
    char* el;

    set->max_element_length = 1;

    for (i = 0; i < length; ++i) {
        el = (char*)malloc(2);
        el[0] = chars[i];
        el[1] = '\0';
        set_add(set, el);
    }

    return set;
}

void set_discard(set_t* set) {
    size_t i;

    for (i = 0; i < set->size; ++i)
        free(set->elements[i]);

    set_destroy(set);
}

void set_print_to_file(set_t* set, FILE* file) {
    size_t i;
    for (i = 0; i < set->size; ++i)
        fprintf(file, "%s\n", set->elements[i]);
}

void generate_with_length(size_t length, FILE* file) {
    set_t* initial_set = get_initial_set();
    set_t* ret = initial_set;
    set_t* tmp;

    size_t i;
    for (i = 1; i < length; ++i) {
        // set_print_to_file(ret, file);
        tmp = set_product(initial_set, ret);
        if (ret != initial_set)
            set_discard(ret);
        ret = tmp;
    }

    set_print_to_file(ret, file);
    set_discard(ret);

    if (ret != initial_set)
        set_discard(initial_set);
}

int main(int argc, char** argv) {
    size_t size = DEFAULT_MAX_LENGTH, i;
    const char* dictionary_file_name = NULL;
    FILE* dictionary_file;

    for (i = 1; i < argc; ++i)
        if (argv[i][0] >= '0' && argv[i][0] <= '9')
            size = strtoul(argv[i], NULL, 10);
        else
            dictionary_file_name = argv[i];

    if (dictionary_file_name != NULL)
        dictionary_file = fopen(dictionary_file_name, "wb+");
    else
        dictionary_file = stdout;

    if (dictionary_file == NULL) {
        perror("Couldn't open dictionary");
        exit(1);
    }

    generate_with_length(size, dictionary_file);

    fclose(dictionary_file);

    return 0;
}
