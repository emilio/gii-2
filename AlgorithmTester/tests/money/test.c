#include <stdlib.h>
#include <stdio.h>

// Different types of coins in cents
typedef size_t coin_type;
const coin_type coins[] = {1, 2, 5, 10, 20, 50, 100, 200};
#define COIN_COUNT (sizeof(coins) / sizeof(coin_type))

/**
 * Returns a size_t vector with the number of coins of each type
 *
 * e.j: for a 2€ coin, it would be: {1,0,0,...,0}
 */
size_t * change(coin_type quantity) {
	// Calloc ensures they are initialized to 0
	size_t * coin_number = calloc(COIN_COUNT, sizeof(size_t));
	size_t i = COIN_COUNT;

	while ( i-- ) {
		if ( quantity >= coins[i] ) {
			coin_number[i] = quantity / coins[i];
			quantity -= coin_number[i] * coins[i];
		}
	}

	return coin_number;
}

void print_change(size_t * change) {
	size_t i = COIN_COUNT;

	while ( i-- ) {
		if ( change[i] ) {
			printf("%zu monedas de %zu céntimos\n", change[i], coins[i]);
		}
	}
}


int main(void) {
	coin_type quantity;
	size_t *number;

	printf("Introduce el dinero (en céntimos): ");
	scanf("%zu", &quantity);

	number = change(quantity);

	print_change(number);

	free(number);

	return 0;
}
