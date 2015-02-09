#ifndef __LISTA_H
#define __LISTA_H

#include "tipoElemento.h"


typedef struct tipoCelda { 
	tipoElemento elemento; 
	struct tipoCelda * sig; 
	} tipoCelda;
	
	
typedef struct {
	tipoCelda * ficticio , *fondo;
	} tipoLista;

typedef tipoCelda *tipoPosicion;

int creaVacia(tipoLista *l);
tipoPosicion primero(tipoLista *l);
tipoPosicion siguiente(tipoPosicion p,tipoLista *l);
tipoPosicion anterior(tipoPosicion p, tipoLista *l);
tipoPosicion fin(tipoLista *l);

int inserta(tipoElemento x, tipoPosicion p, tipoLista *l);
int elimina (tipoPosicion p, tipoLista *l);
int anulaLista(tipoLista *l);

int vacia(tipoLista *l);
tipoPosicion localiza (tipoElemento x, tipoLista *l);
tipoElemento recupera(tipoPosicion p, tipoLista *l);
void imprimeLista(tipoLista *l);
int destruyeLista(tipoLista *l);

#endif 