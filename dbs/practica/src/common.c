#include "common.h"
#include <stdlib.h>
#include <stdio.h>

struct appcom appcom;

void get_str(char* buffer, size_t size) {
    fgets(buffer, size, stdin);
    buffer[size - 1] = '\0';

    if ( *buffer )
        buffer[strlen(buffer) - 1] = '\0';

    fflush(stdin);
}
