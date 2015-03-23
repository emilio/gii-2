#ifndef MACRO_ENSURE_SUCCESS_H_
#define MACRO_ENSURE_SUCCESS_H_

#include <errno.h>
#include <string.h>

/** Multiple arguments = cleanup code */
#define ENSURE(expr, ...) do { \
	if ( ! (expr) ) { \
		fprintf(stderr, "FATAL ERROR: %s:%d\n" \
			"\t%s\n" \
			"\t%s\n", \
			__FILE__, __LINE__, #expr, strerror(errno)); \
		__VA_ARGS__ \
		exit(1); \
	} \
} while ( 0 )

#define ENSURE_EQUALS(expr, value, ...) ENSURE((expr) == (value), __VA_ARGS__)
#define ENSURE_NOT_EQUALS(expr, value, ...) ENSURE((expr) != (value), __VA_ARGS__)
#define ENSURE_SUCCESS(expr, ...) ENSURE_NOT_EQUALS(expr, -1, __VA_ARGS__)
#endif

