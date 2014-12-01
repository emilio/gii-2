#include "Vector.h"

/**
 * Allocate a Vector of size VECTOR_DEFAULT_SIZE
 *
 * @return Vector *
 */
Vector * newVector() {
	Vector * v = (Vector *) malloc(sizeof(Vector));

	return_null_if(v == NULL);

	v->length = 0;
	v->size = 0;

	v->values = (VectorItem *) malloc(VECTOR_DEFAULT_SIZE * sizeof(VectorItem));

	return_null_if(v->values == NULL);

	v->size = VECTOR_DEFAULT_SIZE;

	return v;
}

/**
 * Create a Vector from array of values
 *
 * @param VectorItem * values
 * @param size_t length
 * @param size_t size, if passed 0 or number below length, assumes length * sizeof(VectorItem)
 *
 * @return Vector *
 */
Vector * Vector__from(VectorItem * values, size_t length, size_t size) {
	Vector * v;

	return_null_if(values == NULL);

	v = (Vector *) malloc(sizeof(Vector));

	return_null_if(v == NULL);

	v->values = values;
	v->length = length;

	// This allows assuming size if invalid length
	v->size = MAX(length * sizeof(VectorItem), size);

	return v;
}

/**
 * Destroy a Vector's allocated memory
 *
 * @param Vector * v
 */
void Vector_destroy(Vector *v) {

	return_if(v == NULL);

	free(v->values);
	free(v);
}

/**
 * Extend a vector capacity
 *
 * @param Vector *v
 */
void Vector_extend(Vector *v) {
		v->values = (VectorItem *) realloc(v->values, v->size * 2);
		v->size *= 2;
		return_if(v->values == NULL);
}

/**
 * Set a value in a vector by index
 *
 * @param Vector * v
 * TODO: Review return value
 */
void Vector_set(Vector *v, size_t index, VectorItem value) {
	while ( index >= v->size ) {
		Vector_extend(v);
	}
	v->length = MAX(v->length, index + 1);
	v->values[index] = value;
}

/**
 * Get value of a vector
 *
 * @param Vector *v
 * @param size_t index
 *
 * @return VectorItem
 */
VectorItem Vector_get(Vector *v, size_t index) {
	return_val_if( index >= v->length, (VectorItem) NULL ); // NULL casted gives us flexibility for (almost?) any type
	return v->values[index];
}

/**
 * Push a value to the back of the vector
 *
 * @param Vector *v
 * @param VectorItem value
 */
void Vector_push(Vector *v, VectorItem value) {
	return Vector_set(v, v->length, value);
}

/**
 * Swap two values
 *
 * @param Vector *v
 * @param size_t i1
 * @param size_t i2
 */
void Vector_swap(Vector *v, size_t i1, size_t i2) {
	VectorItem tmp = v->values[i1];
	v->values[i1] = v->values[i2];
	v->values[i2] = tmp;
}

/**
 * Copy values from a vector to another one
 *
 * @param Vector *dest
 * @param Vector *src
 * @param fromIndex index to start copying
 * @param fromSrcIndex index of the src vector to start copying
 * TODO: Allow from - to
 */
void Vector_copyFrom(Vector *dest, Vector *src, size_t fromIndex, size_t fromSrcIndex) {
	size_t needed_size = (src->length - fromSrcIndex) * sizeof(VectorItem);

	return_if(fromSrcIndex && src->length <= fromSrcIndex);

	while ( dest->size < needed_size ) {
		Vector_extend(dest);
	}

	memcpy(dest->values + fromIndex, src->values + fromSrcIndex, (src->length - fromSrcIndex) * sizeof(VectorItem));

	dest->length = MAX(dest->length, fromIndex + src->length - fromSrcIndex);
}
