#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
int * bubbleSort(int * vector, size_t length) {
	int * sorted = IntVector_clone(vector, length);
	size_t i, j;
	int temp; // for swapping

	for ( i = 0; i < length; i++ ) {
		for ( j = 0; j < length - i - 1; j++ ) {
			if ( sorted[j] > sorted[j + 1] ) {
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
 *
 */
void testBubbleSort(size_t length) {
	int * vector = IntVector__generateInRange(length, 0, BUBBLE_SORT_TEST_RANGE);
	int * sorted;

	sorted = bubbleSort(vector, length);

	free(vector);
	free(sorted);
}

int main() {
	AlgorithmTester * tester;
	AlgorithmTesterConfig * config;
	AlgorithmTesterBenchmark * benchmark;

	srand(time(NULL));

	config = AlgorithmTesterConfig__empty();

	config->collection_size = 10000;
	config->min_repetitions = 10;
	config->max_execution_time = 2;

	tester = newAlgorithmTester(testBubbleSort);

	benchmark = AlgorithmTester_test(tester, config);
	AlgorithmTesterBenchmark_toConsole(benchmark);

	free(benchmark);
	free(config);
	free(tester);

	return 0;
}
