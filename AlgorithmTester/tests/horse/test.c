#include <stdlib.h>
#include <stdio.h>

#define MAP_SIZE 8

typedef struct Map {
	size_t width;
	size_t height;
	int positions[MAP_SIZE][MAP_SIZE];
} Map;

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct Horse {
	int movement_count;
	Point current_position;
} Horse;

void printMap(Map *m) {
	size_t i, j;
	for ( i = 0; i < m->width; ++i ) {
		for ( j = 0; j < m->height; ++j ) {
			printf("%d\t", m->positions[i][j]);
		}
		printf("\n");
	}
}

void emptyMap(Map *m) {
	size_t i, j;
	for ( i = 0; i < m->width; ++i )
		for ( j = 0; j < m->height; ++j )
			m->positions[i][j] = 0;
}


int valid_move(const Map *map, const Point *p) {
	if ( p->x < 0 || p->y < 0 || p->x >= map->width || p->y >= map->height )
		return 0;

	return ! map->positions[p->x][p->y];
}

#define TRY_MOVE(i, _x, _y) do { \
	test.x = curr_x + _x; \
	test.y = curr_x + _y; \
	if ( valid_move(map, &test) ) { \
		h->current_position = test; \
		map->positions[test.x][test.y] = ++h->movement_count; \
		return 1; \
	} \
} while (0)

int move(Map *map, Horse *h) {
	int curr_x = h->current_position.x,
		curr_y = h->current_position.y;
	Point test;

	TRY_MOVE(1, 3, 1);
	TRY_MOVE(2, 3, -1);
	TRY_MOVE(3, -3, 1);
	TRY_MOVE(4, -3, -1);

	TRY_MOVE(5, 1, 3);
	TRY_MOVE(6, 1, -3);
	TRY_MOVE(7, -1, 3);
	TRY_MOVE(8, -1, -3);


	return 0;
}

void try(Map *map, Horse *h, Point start) {
	h->current_position = start;
	h->movement_count = 1;

	emptyMap(map);

	map->positions[h->current_position.x][h->current_position.y] = 1;

	printMap(map);

	while( move(map, h) ) {
		printMap(map);
		printf("\n");
	}
}

int main() {
	Map m;
	Horse h;
	m.width = m.height = MAP_SIZE;
	Point p = {0,0};

	try(&m, &h, p);

	return 0;
}
