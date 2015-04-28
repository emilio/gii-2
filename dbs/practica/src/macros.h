#ifndef MACROS_H_
#define MACROS_H_
#include <stdlib.h>
#include <string.h>

void handle_error();

#ifdef POSTGRES
#  define NOT_FOUND ECPG_NOT_FOUND
#else
#  define NOT_FOUND SQLE_NOTFOUND
#endif

#define ARGUMENT_ERROR() do { \
    fprintf(stderr, "Error: Unexpected number of arguments.\n"); \
    fprintf(stderr, "Run with `help` to know more"); \
    exit(1); \
} while (0)

#define COPY_TO_VARCHAR(vchar, str, _len) do { \
    strncpy(vchar.arr, str, _len); \
    vchar.arr[_len - 1] = '\0'; \
    vchar.len = strlen(vchar.arr); \
} while ( 0 )

#endif
