#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tipoLista.h"

int purga(tipoLista* l);
tipoLista duplicaOrdenada(tipoLista* l);

int main(void) {
    tipoLista miLista, listaOrdenada;
    tipoPosicion p;
    int i;

    srandom(time(NULL));

    printf("Creando la lista vacía: %d\n", creaVacia(&miLista));
    p = primero(&miLista);
    for (i = 0; i < 10; i++) {
        inserta(random() % 10, p, &miLista);
        p = siguiente(p, &miLista);
    }

    imprimeLista(&miLista);

    listaOrdenada = duplicaOrdenada(&miLista);
    imprimeLista(&listaOrdenada);
    anulaLista(&listaOrdenada);
    destruyeLista(&listaOrdenada);

    purga(&miLista);
    imprimeLista(&miLista);

    listaOrdenada = duplicaOrdenada(&miLista);
    imprimeLista(&listaOrdenada);

    anulaLista(&miLista);
    destruyeLista(&miLista);

    anulaLista(&listaOrdenada);
    destruyeLista(&listaOrdenada);

    return 0;
}

int purga(tipoLista* l) {
    tipoPosicion act, sig;

    act = primero(l);
    while (act != fin(l)) {
        sig = siguiente(act, l);
        while (sig != fin(l)) {
            if (!compara(recupera(act, l), recupera(sig, l)))
                elimina(sig, l);
            else
                sig = siguiente(sig, l);
        }
        act = siguiente(act, l);
    }
    return 0;
}

tipoLista duplicaOrdenada(tipoLista* l) {
    tipoPosicion act, ins;
    tipoLista res;
    tipoElemento valor;

    act = primero(l);
    valor = recupera(act, l);
    creaVacia(&res);
    ins = primero(&res);
    inserta(valor, ins, &res);

    act = siguiente(act, l);
    while (act != fin(l)) {
        valor = recupera(act, l);
        ins = primero(&res);
        while (ins != fin(&res) && 0 < compara(valor, recupera(ins, &res))) {
            ins = siguiente(ins, &res);
        }
        inserta(valor, ins, &res);
        act = siguiente(act, l);
    }
    return res;
}
