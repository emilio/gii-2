#include <stdlib.h>
#include <stdio.h>
#include "../../AlgorithmTester.h"
#include "../../AlgorithmTester.c"
#define VECTOR_ITEM_TYPE unsigned int
#include "radix.c"

Vector* generate(size_t length) {
    size_t i = 0;
    Vector* v = newVector();
    for (; i < length; i++) {
        Vector_push(v, (VectorItem)rand() % 101 - 50);
    }
    return v;
}

void print(Vector* v) {
    size_t i = 0;
    for (; i < v->length; i++) {
        printf("%u\t", get(v, i));
    }
    printf("\n");
}

int main() {
    srand(time(NULL));
    Vector* unsorted = generate(5);

    print(unsorted);

    radixSort(unsorted);

    print(unsorted);

    return 0;
}
