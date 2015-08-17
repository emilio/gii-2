#include <stdio.h>
#include "tipoElemento.h"

char compara(tipoElemento a, tipoElemento b) {
    return ((a == b) ? 0 : (a < b) ? -1 : 1);
}
