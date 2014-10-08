#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ALGORITHM_TESTER_CONFIG_DEFAULT_MIN_REPETITIONS 10
#define ALGORITHM_TESTER_CONFIG_DEFAULT_MAX_EXECUTION_TIME 2

#include "../../AlgorithmTester.h"
#include "../../AlgorithmTester.c"

size_t A(size_t);
size_t B(size_t);
size_t C(size_t);
size_t D(size_t);
size_t E(size_t);
size_t F(size_t);
size_t G(size_t);
size_t H(size_t);
size_t I(size_t);
size_t J(size_t);

#include "implementation.c" 

#define TESTER(algo, name) void name(size_t n, AlgorithmTesterBenchmark * benchmark, void * data) { \
	ALGORITHM_TESTER_TEST(algo(n), benchmark); \
}

TESTER(A, testA)
TESTER(B, testB)
TESTER(C, testC)
TESTER(D, testD)
TESTER(E, testE)
TESTER(F, testF)
TESTER(G, testG)
TESTER(H, testH)
TESTER(I, testI)
TESTER(J, testJ)

/** Each call to this macro adds two items to the array */
#define REGISTER_ALGORITHM(fn) { #fn, fn },
typedef struct AvailableAlgorithm {
	char * name;
	AlgorithmTesterFunction fn;
} AvailableAlgorithm;

const AvailableAlgorithm ALGORITHMS[] = {
	REGISTER_ALGORITHM(testA)
	REGISTER_ALGORITHM(testB)
	REGISTER_ALGORITHM(testC)
	REGISTER_ALGORITHM(testD)
	REGISTER_ALGORITHM(testE)
	REGISTER_ALGORITHM(testF)
	REGISTER_ALGORITHM(testG)
	REGISTER_ALGORITHM(testH)
	REGISTER_ALGORITHM(testI)
	REGISTER_ALGORITHM(testJ)
};

#define ALGORITHMS_LENGTH (sizeof(ALGORITHMS) / sizeof(AvailableAlgorithm))

void showAvailableAlgorithms() {
	size_t i = 0;

	printf("Available algorithms: \n");
	for ( ; i < ALGORITHMS_LENGTH; i++ ) {
		printf(" * %s\n", ALGORITHMS[i].name);
	}
}

AlgorithmTesterFunction getAlgorithm(const char * name) {
	size_t i = 0;

	for ( ; i < ALGORITHMS_LENGTH; i++ ) {
		if ( strcmp(ALGORITHMS[i].name, name) == 0 ) {
			return ALGORITHMS[i].fn;
		}
	}

	return NULL;
}

int main(int argc, char ** argv) {
	AlgorithmTester * tester;
	AlgorithmTesterFunction algo;
	AlgorithmTesterConfig * config;
	AlgorithmTesterBenchmark * benchmark;
	FILE * results;
	char * file_name;
	int i;

	if ( argc < 3 ) {
		printf("Use: %s algorithm_tester_name collection_size_1 [collection_size_2] [collection_size_3] ...\n", argv[0]);
		showAvailableAlgorithms();		
		return EXIT_FAILURE;
	}

	algo = getAlgorithm(argv[1]);

	if ( algo == NULL ) {
		printf("%s: Algorithm tester %s not found\n", argv[0], argv[1]);
		showAvailableAlgorithms();
		return EXIT_FAILURE;
	}

	tester = newAlgorithmTester(algo);
	config = AlgorithmTesterConfig__default();

	file_name = (char *) malloc(strlen(argv[1]) + 1 + 4);
	strcpy(file_name, argv[1]);
	strcat(file_name, ".txt");

	results = fopen(file_name, "w");

	for ( i = 2; i < argc; i++ ) {
		config->collection_size = strtoul(argv[i], NULL, 10);
		benchmark = AlgorithmTester_test(tester, config, NULL);
		AlgorithmTesterBenchmark_toConsole(benchmark);
		AlgorithmTesterBenchmark_toStreamDelimited(benchmark, results, ';');
		free(benchmark);
	}

	fclose(results);
	free(file_name);
	free(config);
	free(tester);


	return EXIT_SUCCESS;
}
