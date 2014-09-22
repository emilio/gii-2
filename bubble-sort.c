/**
 * Ya, no uso make, pero porque no me da tiempo
 * He perferido implementar la función `test` reutilizable, ya que me servirá durante toda la asignatura
 *
 * @author Emilio Cobos <emiliocobos@usal.es>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy
#include <time.h>

// #define SWAP(x, y, T) do { T temp##x##y = x; x = y; y = temp##x##y; } while (0)
/**
 * Generate a int vector of size `size`
 *
 * @param size_t size;
 * @return int *
 */
int * newIntVector(size_t size) {
	int * ret;

	ret = (int *) malloc(size * sizeof(int));

	return ret;
}

/**
 * Generates a vector of size `size` and fills it with random numbers between `min` and `max`
 *
 * @param size_t length
 * @param size_t min
 * @param size_t max
 *
 * @return int *
 */
int * IntVector__generateInRange(size_t length, size_t min, size_t max) {
	size_t
		i = 0,
		range = max - min;
	int * ret = newIntVector(length);

	for ( ; i < length; i++ ) {
		ret[i] = min + (rand() % range);
	}

	return ret;
}

/**
 * Clones a vector
 *
 * @param int * vector
 * @param size_t length
 *
 * @return int *
 */
int * IntVector_clone(int * vector, size_t length) {
	int * ret = newIntVector(length);

	// Copy it exactly
	memcpy(ret, vector, length * sizeof(int));

	return ret;
}


/**
 * Test a function at least `min_repetitions` times, and during `_max_time` at most
 *
 * @param void (*)(void *) function to test
 * @param size_t _max_time max time in milliseconds
 * @param size_t min_repetitions minimum number of repetitions
 */
void test(void (*func)(void), size_t _max_time, size_t min_repetitions) {
	size_t repetitions = 0;
	clock_t
		max_time = _max_time * CLOCKS_PER_SEC,
		initial_time,
		end_time;
	double total;

	end_time = initial_time = clock();

	while( (end_time - initial_time) < max_time || repetitions < min_repetitions ) {
		repetitions++;
		func();
		end_time = clock();
	}

	total = (end_time - initial_time) / (double) CLOCKS_PER_SEC;

	// Todo: handle results
	printf("Repetitions: %u, Total: %g, Average: %g\n", 
		repetitions,
		total,
		total / repetitions
	);
}


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
 * @param size_t length
 * @param size_t min
 * @param size_t max
 *
 */
void testBubbleSortWith(size_t length, size_t min, size_t max) {
	int * vector = IntVector__generateInRange(length, min, max);
	int * sorted;

	sorted = bubbleSort(vector, length);
}

void test1() {
	testBubbleSortWith(100, 0, 1000);
}

void test2() {
	testBubbleSortWith(1000, 0, 100000);
}

void test3() {
	testBubbleSortWith(10000, 0, 100000);
}

int main() {
	srand(time(NULL));
	// Test test 1 during 2 seconds or 1000 repetitions
	printf("Test 1: \n");
	test(test1, 2, 1000);
	printf("End\n");

	printf("Test 2: \n");
	test(test2, 5, 10000);
	printf("End\n");

	printf("Test 3: \n");
	test(test3, 5, 10000);
	printf("End\n");

	return 0;
}
