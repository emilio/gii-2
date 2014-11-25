#include "Vector.h"
#include "Vector.c"

#define RADIX_NUM_BITS sizeof(VECTOR_ITEM_TYPE) * 8

#define get(v, i) Vector_get(v,i)
#define push(v, i) Vector_push(v,i)

void radixSort_bit(Vector * v, VectorItem mask, char invert) {
	size_t i = 0;
	VectorItem val;
	/** This can be optimized */
	Vector * vector0 = newVector();
	Vector * vector1 = newVector();

	for ( ; i < v->length; i++ ) {
		val = get(v, i);
		if ( val & mask ) {
			push(vector1, val);
		} else {
			push(vector0, val);
		}
	}

	// For the last digit of signed values
	if ( invert ) {
		Vector_copyFrom(v, vector1, 0, 0);
		Vector_copyFrom(v, vector0, vector1->length, 0);
	} else {
		Vector_copyFrom(v, vector0, 0, 0);
		Vector_copyFrom(v, vector1, vector0->length, 0);
	}

	Vector_destroy(vector0);
	Vector_destroy(vector1);
}


void radixSort(Vector * v) {
	static char is_signed_type = ((VectorItem) -1) < 0;
	size_t bits = RADIX_NUM_BITS;
	VectorItem mask = 0x01;

	while ( bits-- ) {
		radixSort_bit(v, mask, is_signed_type && bits == 0);
		mask = mask << 1;
	}
}

#undef get
#undef push
