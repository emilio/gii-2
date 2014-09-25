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
AlgorithmTesterConfig * newAlgorithmTesterConfig(size_t min_repetitions, size_t max_execution_time) {
	AlgorithmTesterConfig * config;

	config = (AlgorithmTesterConfig *) malloc(sizeof(AlgorithmTesterConfig));

	// return_null_if(config == NULL);

	config->min_repetitions = min_repetitions;
	config->max_execution_time = max_execution_time;

	return config;
}

/**
 * AlgorithmTesterConfig constructor without params, more legible. Defaults all params to 0
 *
 * @constructor
 *
 * @return AlgorithmTesterConfig *
 */
AlgorithmTesterConfig * newAlgorithmTesterEmptyConfig() {
	return newAlgorithmTesterConfig(0, 0);
}

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
AlgorithmTesterBenchmark * newAlgorithmTesterBenchmark(AlgorithmTester * tester, size_t repetitions, size_t collection_size, clock_t clocks_used) {
	AlgorithmTesterBenchmark * benchmark;

	// return_null_if(tester == NULL);

	benchmark = (AlgorithmTesterBenchmark *) malloc(sizeof(AlgorithmTesterBenchmark));

	// return_null_if(benchmark == NULL);

	benchmark->tester = tester;
	benchmark->repetitions = repetitions;
	benchmark->collection_size = collection_size;
	benchmark->clocks_used = clocks_used;

	benchmark->time_used = clocks_used / (double) CLOCKS_PER_SEC;
	benchmark->average_time_used = time_used / repetitions;

	return benchmark;
}

/**
 * Default handle for benchmarks
 *
 * @param AlgorithmTesterBenchmark * self
 */
void AlgorithmTesterBenchmark_toConsole(AlgorithmTesterBenchmark * self) {
	printf("Collection size: %u, Repetitions: %u, Total: %g, Average: %g\n",
		self->collection_size,
		self->repetitions,
		self->time_used,
		self->average_time_used);
}

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
AlgorithmTester * newAlgorithmTester(void (*algorithm)(size_t), AlgorithmTesterConfig * config) {
	AlgorithmTester * tester;

	// return_null_if(config == NULL)
	// return_null_if(algorithm == NULL);

	tester = (AlgorithmTester *) malloc(sizeof(AlgorithmTester));

	// return_null_if(tester == NULL);

	tester->algorithm = algorithm;
	tester->config = config;

	return tester;
}

/**
 * Test an algorithm for an specific collection_size
 *
 * @param AlgorithmTester * self tester to use
 * @param size_t collection_size size of the collection
 * 
 * @return AlgorithmTesterBenchmark *
 */
AlgorithmTesterBenchmark * AlgorithmTester_test(AlgorithmTester * self, size_t collection_size) {
	size_t
		repetitions = 0,
		min_repetitions = self->config->min_repetitions;
	clock_t
		max_time = self->config->max_execution_time * CLOCKS_PER_SEC,
		time_used = 0,
		initial_time;

	double total;

	while ( time_used < max_time || repetitions < min_repetitions ) {
		repetitions++;

		// This way is more precise
		initial_time = clock();
		self->algorithm(collection_size);
		time_used += (clock() - initial_time);
	}

	return newAlgorithmTesterBenchmark(self, repetitions, collection_size, time_used);
}
