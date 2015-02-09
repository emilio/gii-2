#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DECK_LENGTH 40

#define FATAL_ERROR(str, ...) do {           \
	fprintf(stderr, str, ##__VA_ARGS__); \
	exit(0);                             \
} while ( 0 )

typedef enum suit {
	SUIT_GOLDS,
	SUIT_SWORDS,
	SUIT_WARDS,
	SUIT_CUPS,
	SUIT_LAST // Added to calculate the suit length (SUIT_LAST - 1)
} suit_t;

const char *suit_representations[] = {
	"oros",
	"espadas",
	"bastos",
	"copas"
};

#define SUIT_COUNT (SUIT_LAST)
#define CARDS_PER_SUIT (DECK_LENGTH / SUIT_COUNT)

typedef struct {
	suit_t suit;
	int value;
} card_t;

typedef unsigned short bool;

typedef struct deck {
	card_t *cards;
	bool *outside_deck;
	size_t size;
	size_t remaining;
} deck_t;

/** Print a cards value */
void card_print(const card_t *card) {
	printf("%3d %s\n", card->value, suit_representations[card->suit]);
}

/** Generates a correctly initialized deck */
deck_t * deck__new() {
	size_t i;
	int value;
	deck_t *deck = (deck_t *) malloc(sizeof(deck_t));

	deck->size = deck->remaining = DECK_LENGTH;

	deck->cards = (card_t *) malloc(sizeof(card_t) * deck->size);
	deck->outside_deck = (bool *) malloc(sizeof(bool) * deck->size);

	memset(deck->outside_deck, 0, sizeof(bool) * deck->size);

	for ( i = 0; i < deck->size; ++i ) {
		value = i % CARDS_PER_SUIT + 1;

		if ( value > 7 )
		       value += 2;

		deck->cards[i].suit = (i / CARDS_PER_SUIT) % SUIT_COUNT;
		deck->cards[i].value = value;
	}

	return deck;
}

/**
 * Get a random card from a deck.
 * Returns a reference to a card of the deck,
 *   and reduces the remaining count
 */
const card_t * deck_card(deck_t *deck) {
	size_t i,
	      index;

	if ( ! deck->remaining )
		return NULL;

	index = (size_t) rand() % deck->remaining;
	deck->remaining--;

	for ( i = 0; i < deck->size; ++i ) {
		if ( ! deck->outside_deck[i] ) {
			if ( index == 0 ) {
				deck->outside_deck[i] = 1;
				return deck->cards + i;
			}
			index--;
		}
	}

	FATAL_ERROR("Corrupt deck\n");

	return NULL; // Not reachable
}

/** Print deck */
void deck_print(const deck_t *deck) {
	size_t i;

	for ( i = 0; i < deck->size; ++i )
		if ( ! deck->outside_deck[i] )
			card_print(deck->cards + i);
}

/** Get card_count random cards from deck: It's a set of pointers ending with null */
const card_t **get_random_cards(deck_t *deck, size_t card_count) {
	size_t i;
	const card_t **ret = malloc(sizeof(card_t *) * (card_count + 1));

	for ( i = 0; i < card_count; ++i )
		ret[i] = deck_card(deck);

	ret[card_count] = NULL;

	return ret;
}

void print_cards(const card_t **cards, size_t len) {
	while ( *cards )
		card_print(*cards++);
}

int main() {
	deck_t *deck = deck__new();
	const card_t **cards_1;
	const card_t **cards_2;

	srand(time(NULL));

	cards_1 = get_random_cards(deck, 20);
	cards_2 = get_random_cards(deck, 10);

	printf("Set 1: \n");
	print_cards(cards_1, 20);

	printf("Set 2: \n");
	print_cards(cards_2, 10);

	printf("Deck\n");
	deck_print(deck);

	return 0;
}
