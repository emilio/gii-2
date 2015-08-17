#include <stdio.h>
#include <stdlib.h>
#include "../../AlgorithmTester.h"
#include "../../AlgorithmTester.c"
#include "Bag.h"
#include "Bag.c"
#include "Vector.c"

// Problema de la mochila sin fraccionamiento
// Solución voraz, no óptima

// Just for testing, static memory
Item items[] = {{"Linterna", 5, 10},
                {"Piedra", 20, 0},
                {"Mechero", 2, 5},
                {"Garrafa", 10, 15},
                {"Cartera", 5, 20},
                {"Portatil", 20, 30}};
#define ITEM_COUNT (sizeof(items) / sizeof(Item))

int sortItemsByValueRatio(const void* v1, const void* v2) {
    Item* item1 = (Item*)v1;
    Item* item2 = (Item*)v2;

    double diff = ((double)item1->value / item1->weight) -
                  ((double)item2->value / item2->weight);

    return diff < 0 ? -1 : (diff > 0 ? 1 : 0);
}

void showAvailableItems() {
    size_t i = 0;
    printf("+----------------+----+----+-----+\n"
           "| Nombre         | P  | V  | V/P |\n"
           "+----------------+----+----+-----+\n");
    for (; i < ITEM_COUNT; i++) {
        printf("| %-14s | %2zu | %2zu | %3zu |\n", items[i].name,
               items[i].weight, items[i].value,
               items[i].value / items[i].weight);
        printf("+----------------+----+----+-----+\n");
    }
}

// Yeah, it's repetitive, but... I'm too lazy right now
void showBagItems(Bag* bag) {
    size_t i = 0;
    Item* item;
    printf("+----------------+----+----+-----+\n"
           "| Nombre         | P  | V  | V/P |\n"
           "+----------------+----+----+-----+\n");
    for (; i < bag->items->length; i++) {
        item = Vector_get(bag->items, i);
        printf("| %-14s | %2zu | %2zu | %3zu |\n", item->name, item->weight,
               item->value, item->value / item->weight);
        printf("+----------------+----+----+-----+\n");
    }
}
int main() {
    Bag* bag = newBag(40); // Capacity = 40
    size_t i = ITEM_COUNT;

    qsort(items, ITEM_COUNT, sizeof(Item), sortItemsByValueRatio);

    showAvailableItems();

    while (i--) {
        if (Bag_supports(bag, &items[i]))
            Bag_addItem(bag, &items[i]);
    }

    printf("Value: %zu\n", bag->value);
    printf("Capacity: %zu / %zu\n", bag->used_capacity, bag->capacity);
    printf("Items:\n");
    showBagItems(bag);

    Bag_destroy(bag);

    return 0;
}
