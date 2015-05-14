#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct appcom appcom = { { 0, "" } };

void get_str(char* buffer, size_t size) {
    printf("> ");
    fgets(buffer, size, stdin);
    buffer[size - 1] = '\0';

    str_trim_right(buffer);
}

char* str_trim_right(char* input) {
    size_t len = strlen(input);

    while ( len-- )
        if ( ! isspace(input[len]) )
            break;

    input[len + 1] = '\0';

    return input;
}

char* str_copy(const char* input) {
    size_t len = strlen(input);
    char* ret = (char*) malloc(len + 1);

    ret[len] = '\0';
    memcpy(ret, input, len);

    return ret;
}

char get_bool() {
    char resp;
    do {
        printf("(y/n): ");
        resp = tolower(getchar());
        FLUSH_STDIN();
    } while ( ! ( resp == 'y' || resp == 'n' ) );

    if ( resp == 'y' )
        return 1;

    return 0;
}

int get_int() {
    int ret;
    printf("> ");
    scanf("%d", &ret);

    FLUSH_STDIN();

    return ret;
}
