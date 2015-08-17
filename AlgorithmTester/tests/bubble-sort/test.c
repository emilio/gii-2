#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ALGORITHM_TESTER_CONFIG_DEFAULT_COLLECTION_SIZE 100
#define ALGORITHM_TESTER_CONFIG_DEFAULT_MIN_REPETITIONS 10
#define ALGORITHM_TESTER_CONFIG_DEFAULT_MAX_EXECUTION_TIME 2

#include "../../AlgorithmTester.h"
#include "./utils/IntVector.h"

// Just for quick compilation
#include "../../AlgorithmTester.c"
#include "./utils/IntVector.c"

#define BUBBLE_SORT_TEST_RANGE 10000

/**
 * Bubble sort algorithm
 *
 * @param int * vector
 * @param size_t length
 *
 * @return int *
 */
int* bubbleSort(int* vector, size_t length) {
    int* sorted = IntVector_clone(vector, length);
    size_t i, j;
    int temp; // for swapping

    for (i = 0; i < length; i++) {
        for (j = 0; j < length - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                // SWAP(sorted[j], sorted[j + 1], int);
                temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    return sorted;
}

/**
 * Test bubble sort
 *
 * @param size_t collection_size
 * @param AlgorithmTesterBenchmark * benchmark
 * @param void * data
 *
 */
void testBubbleSort(size_t length, AlgorithmTesterBenchmark* benchmark,
                    void* data) {
    int* vector = IntVector__generateInRange(length, 0, BUBBLE_SORT_TEST_RANGE);
    int* sorted;

    ALGORITHM_TESTER_TEST(sorted = bubbleSort(vector, length), benchmark);

    free(vector);
    free(sorted);
}

int main(int argc, char** argv) {
    AlgorithmTester* tester;
    AlgorithmTesterBenchmark* benchmark;
    AlgorithmTesterConfig* config;
    FILE* results;

    int i;
    size_t current_size;

    srand(time(NULL));

    if (argc < 2) {
        printf("Use: %s collection_size_1 [collection_size_2] "
               "[collection_size_3] ...\n",
               argv[0]);
        return EXIT_FAILURE;
    }

    tester = newAlgorithmTester(testBubbleSort);
    results = fopen("results.txt", "w");

    config = AlgorithmTesterConfig__default();

    for (i = 1; i < argc; i++) {
        config->collection_size = strtoul(argv[i], NULL, 10);
        benchmark = AlgorithmTester_test(tester, config, NULL);
        AlgorithmTesterBenchmark_toConsole(benchmark);
        AlgorithmTesterBenchmark_toStreamDelimited(benchmark, results, ';');
        free(benchmark);
    }

    fclose(results);
    free(config);
    free(tester);

    return 0;
}
