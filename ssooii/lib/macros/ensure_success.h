#ifndef MACRO_ENSURE_SUCCESS_H_
#define MACRO_ENSURE_SUCCESS_H_

#include <errno.h>
#include <string.h>

#define ENSURE(expr) do { \
	if ( ! (expr) ) { \
		fprintf(stderr, "FATAL ERROR: %s:%d\n" \
			"\t%s\n" \
			"\t%s\n", \
			__FILE__, __LINE__, #expr, strerror(errno)); \
		exit(1); \
	} \
} while ( 0 )

#define ENSURE_EQUALS(expr, value) ENSURE((expr) == (value))
#define ENSURE_NOT_EQUALS(expr, value) ENSURE((expr) != (value))
#define ENSURE_SUCCESS(expr) ENSURE_NOT_EQUALS(expr, -1)
#endif

