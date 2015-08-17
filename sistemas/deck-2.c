#include <stdio.h>
#include <stdlib.h>

void print(char* cad) {
    char temp;
    while (*cad != '\0')
        putchar(*cad++);
    putchar('\n');
}

int sumaVector(int vec[], int longitud) {
    int suma = 0;
    int i;

    for (i = 0; i < longitud; i++) {
        printf("%d\t", vec[i]);
        suma += vec[i];
    }

    return suma;
}

int main() {
    int vector[] = {1, 2, 3, 1, 4};
    int suma;
    char* cadena = "cba";

    suma = sumaVector(vector, 666666);

    printf("El valor de la suma es %d\n", suma);

    return 0;
}
