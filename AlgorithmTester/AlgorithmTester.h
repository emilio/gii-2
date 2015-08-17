#ifndef __ALGORITHM_TESTER_H
#define __ALGORITHM_TESTER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef is_null
#define is_null(a) ((a) == NULL)
#endif

#ifndef return_val_if
#define return_if(expr) return_val_if(expr, )
#define return_if_fail(expr) return_val_if_fail(expr, )
#define return_null_if(expr) return_val_if(expr, NULL)
#define return_null_if_fail(expr) return_val_if_fail(expr, NULL)

#define return_val_if(expr, val)                                               \
    if (expr) {                                                                \
        fprintf(stderr, "[WARNING] [%s:%d:%s] " #expr " passed\n", __FILE__,   \
                __LINE__, __FUNCTION__);                                       \
        return val;                                                            \
    }

#define return_val_if_fail(expr, val)                                          \
    if (!(expr)) {                                                             \
        fprintf(stderr, "[WARNING] [%s:%d:%s] " #expr " failed\n", __FILE__,   \
                __LINE__, __FUNCTION__);                                       \
        return val;                                                            \
    }
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef ALGORITHM_TESTER_CONFIG_DEFAULT_COLLECTION_SIZE
#define ALGORITHM_TESTER_CONFIG_DEFAULT_COLLECTION_SIZE 1
#endif

#ifndef ALGORITHM_TESTER_CONFIG_DEFAULT_MIN_REPETITIONS
#define ALGORITHM_TESTER_CONFIG_DEFAULT_MIN_REPETITIONS 0
#endif

#ifndef ALGORITHM_TESTER_CONFIG_DEFAULT_MAX_EXECUTION_TIME
#define ALGORITHM_TESTER_CONFIG_DEFAULT_MAX_EXECUTION_TIME 0
#endif

#define ALGORITHM_TESTER_START                                                 \
    do {                                                                       \
        clock_t __initial_time = clock();

#define ALGORITHM_TESTER_END(benchmark)                                        \
    benchmark->clocks_used += (clock() - __initial_time);                      \
    benchmark->repetitions++;                                                  \
    }                                                                          \
    while (0)

#define ALGORITHM_TESTER_TEST(expr, benchmark)                                 \
    ALGORITHM_TESTER_START                                                     \
    expr;                                                                      \
    ALGORITHM_TESTER_END(benchmark);

#define ALGORITHM_TESTER_TEST_TO_CONSOLE(algo, config, data)                   \
    do {                                                                       \
        AlgorithmTester* __##algo##_tester = newAlgorithmTester(algo);         \
        AlgorithmTesterBenchmark* __##algo##_benchmark =                       \
            AlgorithmTester_test(__##algo##_tester, config, data);             \
        AlgorithmTesterBenchmark_toConsole(__##algo##_benchmark);              \
        free(__##algo##_benchmark);                                            \
        free(__##algo##_tester);                                               \
    } while (0)

/**
 * Configuration for AlgorithmTester
 */
typedef struct AlgorithmTesterConfig {
    size_t collection_size; // <- Collection size
    size_t min_repetitions; // <- Minimum number of repetitions to test. Set to
                            // 0 to depend only of repetitions
    size_t max_execution_time; // <- Max time to test in seconds per
                               // collection_size. Set to 0 to depend only of
                               // repetitions
} AlgorithmTesterConfig;

/** Type declaration to allow cross reference */
struct AlgorithmTester;

/**
 * Test result
 */
typedef struct AlgorithmTesterBenchmark {
    struct AlgorithmTester* tester; // <- Original tester, just for reference
    size_t collection_size;         // <- Collection size
    size_t repetitions;             // <- Number of repetitions
    clock_t clocks_used;            // <- Time used in clocks
} AlgorithmTesterBenchmark;

/**
 * Tester function to use, receiving:
 * 	- the collection size
 *	- the benchmark (needed to count clocks)
 * 	- custom data passed to the `test` function
 * 	This function needs to call the macros ALGORITHM_TESTER_START and
 *ALGORITHM_TESTER_END(benchmark) or the helper ALGORITHM_TESTER_TEST(expr,
 *benchmark)
 */
typedef void (*AlgorithmTesterFunction)(size_t, AlgorithmTesterBenchmark*,
                                        void*);

/**
 * AlgorithmTester struct
 */
typedef struct AlgorithmTester {
    AlgorithmTesterFunction algorithm; // <- Algorithm to test receiving a
                                       // collection size, benchmark, and data
                                       // as parameters
} AlgorithmTester;

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
AlgorithmTesterConfig* newAlgorithmTesterConfig(size_t collection_size,
                                                size_t min_repetitions,
                                                size_t max_execution_time);

/**
 * AlgorithmTesterConfig constructor without params, more legible. Defaults all
 *params to 0
 *
 * @constructor
 *
 * @return AlgorithmTesterConfig *
 */
AlgorithmTesterConfig* AlgorithmTesterConfig__empty();

/**
 * Get default config
 *
 * @constructor
 *
 * @return AlgorithmTesterConfig *
 */
AlgorithmTesterConfig* AlgorithmTesterConfig__default();

/**
 * Gets config from array of strings.
 * If not present fallback to default
 *
 * @constructor
 *
 * @param char ** argv arguments: collection_size min_repetitions
 *max_execution_time
 * @param size_t argc argument count
 *
 */
AlgorithmTesterConfig* AlgorithmTesterConfig__fromArgs(char** argv,
                                                       size_t argc);

/**
 * Get config from shell args
 *
 * @param int argc
 * @param char ** argv
 *
 * @see AlgorithmTesterConfig__fromArgs()
 *
 * @return AlgorithmTesterConfig *
 */
AlgorithmTesterConfig* AlgorithmTesterConfig__fromShellArgs(int argc,
                                                            char** argv);

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
AlgorithmTesterBenchmark* newAlgorithmTesterBenchmark(AlgorithmTester* tester,
                                                      size_t repetitions,
                                                      size_t collection_size,
                                                      clock_t clocks_used);

/**
 * Returns benchmark used time in seconds
 *
 * @param AlgorithmTesterBenchmark * self
 */
double AlgorithmTesterBenchmark_totalTime(AlgorithmTesterBenchmark* self);

/**
 * Returns benchmark used time per repetition in seconds
 *
 * @param AlgorithmTesterBenchmark * self
 */
double AlgorithmTesterBenchmark_averageTime(AlgorithmTesterBenchmark* self);

/**
 * Default handle for benchmarks
 *
 * @param AlgorithmTesterBenchmark * self
 */
void AlgorithmTesterBenchmark_toConsole(AlgorithmTesterBenchmark* self);

/**
 * Print benchmark data to stream delimited by `sep`
 *
 * @param AlgorithmTesterBenchmark * self
 * @param FILE * stream
 * @param const char sep
 */
void AlgorithmTesterBenchmark_toStreamDelimited(AlgorithmTesterBenchmark* self,
                                                FILE* stream, const char sep);

/**
 * AlgorithmTester constructor
 *
 * @param AlgorithmTesterFunction algorithm
 *
 * @constructor
 *
 * @return AlgoritmTester *
 */
AlgorithmTester* newAlgorithmTester(AlgorithmTesterFunction algorithm);

/**
 * Test an algorithm for an specific collection_size
 *
 * @param AlgorithmTester * self tester to use
 * @param AlgorithmTesterConfig * config
 * @param void * data custom data passed to the tester function
 *
 * @return AlgorithmTesterBenchmark *
 */
AlgorithmTesterBenchmark* AlgorithmTester_test(AlgorithmTester* self,
                                               AlgorithmTesterConfig* config,
                                               void* data);
#endif
