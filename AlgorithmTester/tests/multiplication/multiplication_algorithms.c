/**
 * Multiply by the russian method
 *
 * @param long
 * @param long
 *
 * @return long
 */
long russianMultiplication(long factor_1, long factor_2) {
	long ret = 0;

	while ( factor_1 ) {
		if ( factor_1 % 2 ) {
			ret += factor_2;
		}

		factor_1 /= 2;
		factor_2 *= 2;
	}

	return ret;
}

/**
 * Multiply by the classic method
 *
 * @param long
 * @param long
 *
 * @return long
 */
long classicMultiplication(long factor_1, long factor_2) {
	long ret = 0,
		temp;
	int digits;

	if ( factor_1 == 0 || factor_2 == 0 ) {
		return 0; // With previous implementation of DIGITS() when 0 was given we had troubles (log(0) doesn't exists)
	}

	digits = DIGITS(factor_2);
	while ( digits-- ) {
		temp = factor_1 * (factor_2 / (long) pow(10, digits) % 10);
		ret += ((long) pow(10, digits)) * temp;
	}

	return ret;
}

/**
 * Multiply by the divide method
 *
 * @param long
 * @param long
 *
 * @return long
 */
long divideMultiplication(long factor_1, long factor_2) {
	int digits_1 = DIGITS(factor_1),
		digits_2 = DIGITS(factor_2),
		max_digits = MAX(digits_1, digits_2);
	long a,b,c,d,
		exp;

	if ( max_digits < 2 ) {
		return classicMultiplication(factor_1, factor_2);
	}

	max_digits /= 2;
	exp = (long) pow(10, max_digits);

	a = factor_1 / exp;
	b = factor_1 % exp;
	c = factor_2 / exp;
	d = factor_2 % exp;

	return	divideMultiplication(a, c) +
			divideMultiplication(a, d) +
			divideMultiplication(b, c) +
			divideMultiplication(b, d);
	
}

/**
 * Multiply by the iterative method
 *
 * @param long
 * @param long
 *
 * @return long
 */
long iterativeMultiplication(long factor_1, long factor_2) {
	long ret = 0;

	while ( factor_2-- ) {
		ret += factor_1;
	}

	return ret;
}
