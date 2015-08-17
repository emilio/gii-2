#include <stdio.h>
#include <stdlib.h>

typedef char bool;
#define false(0)
#define true(1)
#define ALIVE_CHAR 'x'
#define DEAD_CHAR '_'
#define is_alive(f, x, y) (f->m[x][y])

#define DEFAULT_WIDHT 30
#define DEFAULT_HEIGHT DEFAULT_WIDHT
#define DEFAULT_MAX_ROUNDS 0

typedef struct Field {
    bool** m;
    size_t w;
    size_t h;
} Field;

void cls() { printf("\e[1;1H\e[2J"); }

Field* newField(size_t w, size_t h) {
    Field* ret = (Field*)malloc(sizeof(Field));
    size_t i;

    ret->w = w;
    ret->h = h;

    ret->m = malloc(sizeof(bool*) * w);

    for (i = 0; i < w; ++i)
        ret->m[i] = malloc(sizeof(bool) * h);

    return ret;
}

void Field_destroy(Field* f) {
    size_t i;
    for (i = 0; i < f->w; ++i)
        free(f->m[i]);

    free(f->m);

    free(f);
}

void Field_print(Field* f) {
    size_t i, j;
    for (i = 0; i < f->w; ++i) {
        for (j = 0; j < f->h; ++j)
            if (f->m[i][j])
                putchar(ALIVE_CHAR);
            else
                putchar(DEAD_CHAR);
        putchar('\n');
    }
    putchar('\n');
}

void Field_randomize(Field* f) {
    size_t i, j;
    for (i = 0; i < f->w; ++i)
        for (j = 0; j < f->h; ++j)
            f->m[i][j] = rand() % 2;
}

size_t alive_neighbours(Field* f, size_t x, size_t y) {
    size_t alive = 0;
    int i, j;

    for (i = -1; i <= 1; ++i)
        for (j = -1; j <= 1; ++j)
            if (!(i == 0 && j == 0) && x + i < f->w && y + j < f->h &&
                is_alive(f, x + i, y + j))
                alive++;

    return alive;
}

bool next_state(Field* f, size_t x, size_t y) {
    size_t alive = alive_neighbours(f, x, y);

    if (is_alive(f, x, y)) {
        if (alive > 1 && alive < 4)
            return true;
    } else {
        if (alive == 3)
            return true;
    }

    return false;
}

void Field_process(Field* f) {
    size_t i, j;
    for (i = 0; i < f->w; ++i)
        for (j = 0; j < f->h; ++j)
            f->m[i][j] = next_state(f, i, j);
}

int game(const size_t w, const size_t h, const size_t max_rounds) {
    size_t round = 1;
    Field* f = newField(w, h);

    Field_randomize(f);

    do {
        Field_print(f);
        Field_process(f);
        usleep(100 * 1000); // 1/10 seconds
        cls();
        ++round;
    } while (!max_rounds || round < max_rounds);
}

int main(int argc, const char** argv) {
    size_t width = DEFAULT_WIDHT;
    size_t height = DEFAULT_HEIGHT;
    size_t max_rounds = DEFAULT_MAX_ROUNDS;
    srand(time(NULL));

    return game(width, height, max_rounds);
}
