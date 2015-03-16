#ifndef MACROS_FATAL_ERROR_H_
#define MACROS_FATAL_ERROR_H_

#define FATAL_ERROR(msg, ...) do { \
	fprintf(stderr, "(fatal)" msg "\n", ## __VA_ARGS__); \
	exit(1); \
} while(0)

#endif
