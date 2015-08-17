#ifndef __INT_VECTOR_H
#define __INT_VECTOR_H

#include <string.h>

typedef int* IntVector;

/**
 * Allocate a new int vector
 * @param size_t length
 *
 * @return IntVector
 */
IntVector newIntVector(size_t length);

/**
 * Create a vector of `length` elements between `min` and `max`
 *
 * @param size_t length
 * @param int min
 * @param int max
 *
 * @return IntVector
 */
IntVector IntVector__generateInRange(size_t length, int min, int max);

/**
 * Clone an int vector
 *
 * @param IntVector vector
 * @param size_t length
 *
 * @return IntVector
 */
IntVector IntVector_clone(IntVector vector, size_t length);

#endif
