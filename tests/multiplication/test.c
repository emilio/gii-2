#include <stdlib.h>
#include <stdio.h>

#define ALGORITHM_TESTER_CONFIG_DEFAULT_COLLECTION_SIZE 0
#define ALGORITHM_TESTER_CONFIG_DEFAULT_MIN_REPETITIONS 100
#define ALGORITHM_TESTER_CONFIG_DEFAULT_MAX_EXECUTION_TIME 3

#include "../../AlgorithmTester.h"
#include "multiplication_algorithms.h"

// For quick compilation
#include "../../AlgorithmTester.c"
#include "multiplication_algorithms.c"

#define DEFAULT_FACTOR_1 15543
#define DEFAULT_FACTOR_2 54393

#define TESTER_FN(name, fn) void name(size_t size, AlgorithmTesterBenchmark * benchmark, void * data) { \
	long * factors = (long *) data; \
	ALGORITHM_TESTER_TEST(fn(factors[0], factors[1]), benchmark); \
}

TESTER_FN(testClassicMultiplication, classicMultiplication)
TESTER_FN(testRussianMultiplication, russianMultiplication)
TESTER_FN(testDivideMultiplication, divideMultiplication)
TESTER_FN(testIterativeMultiplication, iterativeMultiplication)


int main(int argc, char ** argv) {
	AlgorithmTester * tester = newAlgorithmTester(NULL); // Algorithm changing on runtime
	AlgorithmTesterConfig * config = AlgorithmTesterConfig__fromShellArgs(argc, argv);
	AlgorithmTesterBenchmark * benchmark;

	// factors passed as data to the test function
	long factors[2];

	// Use fourth and fifth parameters if found as factors
	factors[0] = (argc > 4) ? strtol(argv[4], NULL, 10) : DEFAULT_FACTOR_1;
	factors[1] = (argc > 5) ? strtol(argv[5], NULL, 10) : DEFAULT_FACTOR_2;

	// Helper macro to avoid code repetition
	#define TEST(testerFunc) do { \
		tester->algorithm = testerFunc; \
		benchmark = AlgorithmTester_test(tester, config, factors); \
		AlgorithmTesterBenchmark_toConsole(benchmark); \
		free(benchmark); \
	} while(0)

	// Classic
	TEST(testClassicMultiplication);

	// Russian
	TEST(testRussianMultiplication);

	// Divide
	TEST(testDivideMultiplication);

	// Iterative
	TEST(testIterativeMultiplication);

	// free(benchmark); already free'd
	free(config);
	free(tester);

	return 0;
}
