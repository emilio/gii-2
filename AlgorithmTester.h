#ifndef __ALGORITHM_TESTER_H
#define __ALGORITHM_TESTER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Configuration for AlgorithmTester
 */
typedef struct AlgorithmTesterConfig {
	size_t min_repetitions; // <- Minimum number of repetitions to test. Set to 0 to depend only of repetitions
	size_t max_execution_time; // <- Max time to test in seconds per collection_size. Set to 0 to depend only of repetitions
} AlgorithmTesterConfig;

/**
 * AlgorithmTester struct
 */
typedef struct AlgorithmTester {
	void (*algorithm)(size_t); // <- Algorithm to test receiving a collection size as parameter
	AlgorithmTesterConfig * config; // <- Configuration (@see AlgorithmTesterConfig)
} AlgorithmTester;

/**
 * Test result
 */
typedef struct AlgorithmTesterBenchmark {
	AlgorithmTester * tester; // <- Original tester, just for reference
	size_t collection_size; // <- Collection size
	size_t repetitions; // <- Number of repetitions
	clock_t clocks_used; // <- Time used in clocks
	double time_used; // <- Time used in seconds
	double average_time_used; // <- Average time used in seconds
} AlgorithmTesterBenchmark;

/**
 * AlgorithmTesterConfig constructor
 *
 * @param size_t min_repetitions
 * @param size_t max_execution_time
 *
 * @constructor
 *
 * @return AlgorithmTesterConfig *
 */
AlgorithmTesterConfig * newAlgorithmTesterConfig(size_t min_repetitions, size_t max_execution_time);

/**
 * AlgorithmTester constructor
 *
 * @param void (*)(size_t) algorithm algorithm tester function to use, receiving the collection size
 * @param AlgorithmTesterConfig * config
 *
 * @see newAlgorithmTesterConfig
 *
 * @constructor
 *
 * @return AlgoritmTesterConfig *
 */
AlgorithmTester * newAlgorithmTester(void (*algorithm)(size_t), AlgorithmTesterConfig * config);

/**
 * AlgorithmTesterBenchmark constructor
 *
 * @param AlgorithmTester * tester
 * @param size_t repetitions
 * @param size_t collection_size
 * @param clock_t clocks_used
 *
 * @constructor
 *
 * @return AlgorithmTesterBenchmark *
 */
AlgorithmTesterBenchmark * newAlgorithmTesterBenchmark(AlgorithmTester * tester, size_t repetitions, size_t collection_size, clock_t clocks_used);

/**
 * Test a function at least `min_repetitions` times, and during `_max_time` at most
 *
 * @param AlgorithmTester * self tester to use
 * @param size_t collection_size size of the collection
 * 
 * @return AlgorithmTesterBenchmark *
 */
AlgorithmTesterBenchmark * AlgorithmTester_test(AlgorithmTester * self, size_t collection_size);

#endif
