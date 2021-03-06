#ifndef __MULTIPLICATION_ALGORITHMS_H
#define __MULTIPLICATION_ALGORITHMS_H
#include <math.h>

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(a) (((a) > 0) ? (a) : (-(a)))
#endif

#ifndef DIGITS
#define DIGITS(num) ((num == 0) ? (1) : (((int)log10(ABS(num))) + 1))
#endif

/**
 * Multiply by the russian method
 *
 * @param long
 * @param long
 *
 * @return long
 */
long russianMultiplication(long, long);

/**
 * Multiply by the classic method
 *
 * @param long
 * @param long
 *
 * @return long
 */
long classicMultiplication(long, long);

/**
 * Multiply by the divide method
 *
 * @param long
 * @param long
 *
 * @return long
 */
long divideMultiplication(long, long);

/**
 * Multiply by the iterative method
 *
 * @param long
 * @param long
 *
 * @return long
 */
long iterativeMultiplication(long, long);

#endif
