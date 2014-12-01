#ifndef __VECTOR_H
#define __VECTOR_H

#include <string.h>

/** Allow external changes */
#ifndef VECTOR_ITEM_TYPE
#define VECTOR_ITEM_TYPE void *
#endif

#ifndef VECTOR_DEFAULT_SIZE
#define VECTOR_DEFAULT_SIZE 10
#endif

typedef VECTOR_ITEM_TYPE VectorItem;

/** Vector structure */
typedef struct Vector {
	size_t length;
	size_t size;
	VectorItem * values;
} Vector;

/**
 * Allocate a Vector of size VECTOR_DEFAULT_SIZE
 *
 * @return Vector *
 */
Vector * newVector();

/**
 * Create a Vector from array of values
 *
 * @param VectorItem * values
 * @param size_t length
 * @param size_t size, if passed 0 or number below length, assumes length * sizeof(VectorItem)
 *
 * @return Vector *
 */
Vector * Vector__from(VectorItem * values, size_t length, size_t size);

/**
 * Destroy a Vector's allocated memory
 *
 * @param Vector * v
 */
void Vector_destroy(Vector *v);

/**
 * Set a value in a vector by index
 *
 * @param Vector * v
 * TODO: Review return value
 */
void Vector_set(Vector *v, size_t index, VectorItem value);

/**
 * Get value of a vector
 *
 * @param Vector *v
 * @param size_t index
 *
 * @return VectorItem
 */
VectorItem Vector_get(Vector *v, size_t index);

/**
 * Push a value to the back of the vector
 *
 * @param Vector *v
 * @param VectorItem value
 */
void Vector_push(Vector *v, VectorItem value);

/**
 * Swap two values
 *
 * @param Vector *v
 * @param size_t i1
 * @param size_t i2
 */
void Vector_swap(Vector *v, size_t i1, size_t i2);

/**
 * Copy values from a vector to another one
 *
 * @param Vector *dest
 * @param Vector *src
 * @param fromIndex index to start copying
 * @param fromSrcIndex index of the src vector to start copying
 * TODO: Allow from - to
 */
void Vector_copyFrom(Vector *dest, Vector *src, size_t fromIndex, size_t fromSrcIndex);

#endif
