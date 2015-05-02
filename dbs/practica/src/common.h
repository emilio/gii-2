#ifndef MACROS_H_
#define MACROS_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define APPCOM_RET_STR_MAX 70
struct appcom {
    struct {
        int int_value;
        char str_value[APPCOM_RET_STR_MAX];
    } ret;
};

void handle_error();
extern struct appcom appcom;

#define APPCOM_RET_INT(val) do { \
    appcom.ret.int_value = val; \
    snprintf(appcom.ret.str_value, APPCOM_RET_STR_MAX, "%d", val); \
} while (0)

#define CALL(fn, ...) do { \
    int argc__ = 0; \
    char* argv__[] = { __VA_ARGS__, NULL }; \
    char** argvp__ = argv__; \
    while ( *argvp__++ ) argc__++; \
    fn(argc__, argv__); \
} while ( 0 )

#ifdef POSTGRES
#  define NOT_FOUND ECPG_NOT_FOUND
#else
#  define NOT_FOUND SQLE_NOTFOUND
#endif

#define ARGUMENT_ERROR() do { \
    fprintf(stderr, "Error: Unexpected number of arguments.\n"); \
    fprintf(stderr, "Run with `help` to know more\n"); \
    exit(1); \
} while (0)

#define COPY_TO_VARCHAR(vchar, str, _len) do { \
    strncpy(vchar.arr, str, _len); \
    vchar.arr[_len - 1] = '\0'; \
    vchar.len = strlen(vchar.arr); \
} while ( 0 )

void get_str(char*, size_t);
#endif
