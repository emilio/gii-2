#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "tipoLista.h"



int
creaVacia(tipoLista *l)
{
	if (l == NULL)
		return -2;
	else if (NULL == (l->ficticio = l->fondo = (tipoCelda *)malloc(sizeof(tipoCelda))))
		return -1;
	else {
		l->ficticio->sig = NULL;
		return 0;
	}
}


int inserta(tipoElemento x, tipoPosicion p, tipoLista *l) {
	tipoCelda *celda = (tipoCelda *) malloc(sizeof(tipoCelda));

	if ( celda == NULL )
		return -1;

	celda->elemento = x;

	celda->sig = p->sig;

	p->sig = celda;

	if ( p == fin(l) )
		l->fondo = celda;

	return 0;
}


int elimina (tipoPosicion p, tipoLista *l) {
	tipoPosicion a_borrar = p->sig;
	p->sig = a_borrar->sig;

	if ( a_borrar == l->fondo )
		l->fondo = p;

	free(a_borrar);

	return 0;
}



int
anulaLista(tipoLista *l)
{
	tipoPosicion it = primero(l);

	while ( it != fin(l) )
		elimina(it, l);

	return 0;
}




tipoPosicion
primero(tipoLista *l)
{
	if (l == NULL) {
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return NULL;
	}
	else if (l->ficticio == NULL){
		fprintf(stderr,"Fallo precondición lista no inicializada!\n");
		return NULL;
	} else
		return l->ficticio;
}


tipoPosicion
siguiente(tipoPosicion p,tipoLista *l)
{
	if (l == NULL) {
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return NULL;
	}
	else if (p == NULL){
		fprintf(stderr,"Fallo precondición posición no válida!\n");
		return l->fondo;
	}
	else if (p->sig != NULL)
		return p->sig;
	else
		return l->fondo;
}


tipoPosicion
anterior(tipoPosicion p, tipoLista *l)
{
	tipoCelda *anterior;

	if (l == NULL) {
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return NULL;
	}
	else if (p == NULL){
		fprintf(stderr,"Fallo precondición posición no válida!\n");
		return l->ficticio;
	}
	else {
		anterior = l->ficticio;
		while (anterior->sig != p)
			anterior = anterior->sig;
		return anterior;
	}
}

tipoPosicion
fin(tipoLista *l)
{
	if (l == NULL) {
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return NULL;
	}
	else if (l->fondo == NULL){
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return NULL;
	}
	else
		return l->fondo;
}





tipoPosicion
localiza (tipoElemento x, tipoLista *l)
{
	tipoCelda *aDevolver;

	if (l == NULL) {
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return NULL;
	}
	else if (l->ficticio == NULL) {
		fprintf(stderr,"Fallo precondición nodo ficticio no existente!\n");
		return NULL;
	}
	else if (x < 0){
		fprintf(stderr,"Fallo precondición valor x: %d no válido!\n", x);
		return l->fondo;
	}
	else {
		aDevolver = l->ficticio;
		while (aDevolver->sig != NULL  &&  aDevolver->sig->elemento != x)
			aDevolver = aDevolver->sig;
		return aDevolver;
	}
}



tipoElemento
recupera(tipoPosicion p, tipoLista *l)
{
	if (l == NULL) {
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return -1;
	}
	else if (p == NULL){
		fprintf(stderr,"Fallo precondición posición no válida!\n");
		return -2;
	}
	else if (p == l->fondo){
		fprintf(stderr,"Fallo precondición posición no válida\n");
		return -3;
	}
	else
		return p->sig->elemento;
}



void
imprimeLista(tipoLista *l)
{
	tipoCelda *aImprimir;
	int posicion;

	if (l == NULL) {
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return;
	}
	else if (l->ficticio == NULL ){
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return;
	}
	else {
		posicion = 1;
		aImprimir = l->ficticio->sig;
		while (aImprimir != NULL) {
			printf("Valor: %d en posición: %d\n",aImprimir->elemento, posicion++);
			aImprimir = aImprimir->sig;
		}
		printf("%d valores en la lista\n",posicion-1);
	}
}

int
vacia(tipoLista *l)
{
	if (l == NULL) {
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return 0;
	}
	else if (l->ficticio == NULL ){
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return 0;
	}
	else if (l->ficticio->sig == NULL)
		return 1;
	else
		return 0;
}

int
destruyeLista(tipoLista *l)
{
	if (l == NULL) {
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return -1;
	}
	else if (l->ficticio == NULL ){
		fprintf(stderr,"Fallo precondición lista no existente!\n");
		return -2;
	}
	else if (l->ficticio->sig != NULL){
		fprintf(stderr,"Fallo precondición lista no anulada. Use anulaLista()!\n");
		return -3;
	}
	else {
		free(l->ficticio);
		l->ficticio = l->fondo = NULL;
		return 0;
	}
}
