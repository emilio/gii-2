/**
 * AlgorithmTesterConfig constructor
 *
 * @param size_t collection_size
 * @param size_t min_repetitions
 * @param size_t max_execution_time
 *
 * @constructor
 *
 * @return AlgorithmTesterConfig *
 */
AlgorithmTesterConfig * newAlgorithmTesterConfig(size_t collection_size, size_t min_repetitions, size_t max_execution_time) {
	AlgorithmTesterConfig * config;

	config = (AlgorithmTesterConfig *) malloc(sizeof(AlgorithmTesterConfig));

	return_null_if(config == NULL);

	config->collection_size = collection_size;
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
AlgorithmTesterConfig * AlgorithmTesterConfig__empty() {
	return newAlgorithmTesterConfig(0, 0, 0);
}

/**
 * Get default config
 *
 * @constructor
 *
 * @return AlgorithmTesterConfig *
 */
AlgorithmTesterConfig * AlgorithmTesterConfig__default() {
	return newAlgorithmTesterConfig(ALGORITHM_TESTER_CONFIG_DEFAULT_COLLECTION_SIZE, ALGORITHM_TESTER_CONFIG_DEFAULT_MIN_REPETITIONS, ALGORITHM_TESTER_CONFIG_DEFAULT_MAX_EXECUTION_TIME);
}

/**
 * Gets config from array of strings. Recommended use: argc and argv
 * If not present defaults to default
 *
 * @constructor
 *
 * @param char ** argv arguments: collection_size min_repetitions max_execution_time
 * @param size_t argc argument count
 *
 */
AlgorithmTesterConfig * AlgorithmTesterConfig__fromArgs(char ** argv, size_t argc) {
	AlgorithmTesterConfig * ret;

	return_null_if(argc > 3);

	ret = AlgorithmTesterConfig__empty();

	ret->collection_size = (argc) ? strtoul(argv[0], NULL, 10) : ALGORITHM_TESTER_CONFIG_DEFAULT_COLLECTION_SIZE;
	ret->min_repetitions = (argc > 1) ? strtoul(argv[1], NULL, 10) : ALGORITHM_TESTER_CONFIG_DEFAULT_MIN_REPETITIONS;
	ret->max_execution_time = (argc > 2) ? strtoul(argv[2], NULL, 10) : ALGORITHM_TESTER_CONFIG_DEFAULT_MAX_EXECUTION_TIME;

	return ret;
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

	return_null_if(tester == NULL);

	benchmark = (AlgorithmTesterBenchmark *) malloc(sizeof(AlgorithmTesterBenchmark));

	return_null_if(benchmark == NULL);

	benchmark->tester = tester;
	benchmark->repetitions = repetitions;
	benchmark->collection_size = collection_size;
	benchmark->clocks_used = clocks_used;

	return benchmark;
}

/**
 * Returns benchmark used time in seconds
 *
 * @param AlgorithmTesterBenchmark * self
 */
double AlgorithmTesterBenchmark_totalTime(AlgorithmTesterBenchmark * self) {
	return self->clocks_used / (double) CLOCKS_PER_SEC;
}

/**
 * Returns benchmark used time per repetition in seconds
 *
 * @param AlgorithmTesterBenchmark * self
 */
double AlgorithmTesterBenchmark_averageTime(AlgorithmTesterBenchmark * self) {
	return AlgorithmTesterBenchmark_totalTime(self) / self->repetitions;
}

/**
 * Default handle for benchmarks
 *
 * @param AlgorithmTesterBenchmark * self
 */
void AlgorithmTesterBenchmark_toConsole(AlgorithmTesterBenchmark * self) {
	printf("Collection size: %u, Repetitions: %u, Total time (s): %g, Average time (s): %g\n",
		self->collection_size,
		self->repetitions,
		AlgorithmTesterBenchmark_totalTime(self),
		AlgorithmTesterBenchmark_averageTime(self));
}

/**
 * Print benchmark data to stream delimited by `sep`
 *
 * @param AlgorithmTesterBenchmark * self
 * @param FILE * stream
 * @param char sep
 */
void AlgorithmTesterBenchmark_toStreamDelimited(AlgorithmTesterBenchmark * self, FILE * stream, char sep) {
	fprintf(stream, "%u%c%u%c%f%c%f\n",
		self->collection_size,
		sep,
		self->repetitions,
		sep,
		AlgorithmTesterBenchmark_totalTime(self),
		sep,
		AlgorithmTesterBenchmark_averageTime(self));
}

/**
 * AlgorithmTester constructor
 *
 * @param void (*)(size_t) algorithm algorithm tester function to use, receiving the collection size
 *
 * @constructor
 *
 * @return AlgoritmTester *
 */
AlgorithmTester * newAlgorithmTester(void (*algorithm)(size_t)) {
	AlgorithmTester * tester;

	return_null_if(algorithm == NULL);

	tester = (AlgorithmTester *) malloc(sizeof(AlgorithmTester));

	return_null_if(tester == NULL);

	tester->algorithm = algorithm;

	return tester;
}

/**
 * Test an algorithm for an specific collection_size
 *
 * @param AlgorithmTester * self tester to use
 * @param AlgorithmTesterConfig * config
 * 
 * @return AlgorithmTesterBenchmark *
 */
AlgorithmTesterBenchmark * AlgorithmTester_test(AlgorithmTester * self, AlgorithmTesterConfig * config) {
	size_t repetitions = 0;
	clock_t
		max_clocks = config->max_execution_time * CLOCKS_PER_SEC,
		initial_time;
	
	AlgorithmTesterBenchmark * benchmark = newAlgorithmTesterBenchmark(self, 0, config->collection_size, 0); // tester, repetitions, collection_size, clocks_used

	while ( benchmark->clocks_used < max_clocks || benchmark->repetitions < config->min_repetitions ) {
		benchmark->repetitions++;

		// This way is more precise
		initial_time = clock();
		self->algorithm(config->collection_size);
		benchmark->clocks_used += (clock() - initial_time);
	}

	return benchmark;
}
