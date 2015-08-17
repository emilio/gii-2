#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TOP 10000

int main() {
    int bottom = -1, top = TOP + 1, temp;
    char c;

    printf("Pick a number between %d and %d...\n", bottom + 1, top - 1);
    sleep(1);
    printf("Ready?\n");

    do {
        temp = bottom + (top - bottom) / 2;

        if (temp == bottom || temp == top) {
            printf("C'mon dude...\n");
            return 1;
        }

        do {
            printf("%d: ", temp);
            while ((c = getchar()) == '\n')
                ;
        } while (c != '=' && c != '<' && c != '>');

        switch (c) {
        case '=':
            printf("I knew it! The number is %d\n", temp);
            return 0;
        case '<':
            top = temp;
            break;
        case '>':
            bottom = temp;
            break;
        }
    } while (1);

    return 0;
}
