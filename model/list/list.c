#include "list.h"
#include <stdlib.h>
#include <memory.h>


List create() {
	List l;

	int i;

	//Demanem memòria pel primer fantasma
	l.first = (Node *) malloc (sizeof (Node));

	//Demanem memòria pel darrer fantasma
	l.last = (Node *) malloc (sizeof (Node));

	//Inicialitzacions
	for (i = 0; i < MAX_SORTING; i++) {
		l.poi[i] = l.first;
		l.first -> next[i] = l.last;
		l.first -> prev[i] = NULL;
		l.last -> next[i] = NULL;
		l.last -> prev[i] = l.first;
	}

	l.n = 0;

	return l;
}

void insert(List * l, Destination d) {
	int i;
	int condicio;


	//Demanem memòria pel nou node i comprovem si n'hem obtingut
	Node * nou  = (Node *) malloc (sizeof (Node));

	//Copiem l'element a inserir al nou node
	nou->dest = d;

	//Per cadascun dels camps d'ordenació, ordenarem el node
	for (i = 0; i < MAX_SORTING; i++) {
		//En primer lloc, situem el pdi al primer fantasma
		l->poi[i] = l->first;

		do {
			//Avancem al següent element i mirem si aquest encara l'hem de saltar
			l->poi[i] = l->poi[i]->next[i];

			if (l->poi[i] == l->last) {
				break;
			}

			//Les condicions per saltar un element depenen del mode d'ordenació
			switch (i) {
				case BY_NAME:
					condicio = strcmp(d.name, l->poi[i]->dest.name) > 0;
					break;
				case BY_AVG_PRICE:
					condicio = getAverageHotelPrice(d) > getAverageHotelPrice(l->poi[i]->dest);
					break;
				case BY_DISTANCE:
					condicio = getDistanceFromBcn(d) > getDistanceFromBcn(l->poi[i]->dest);
					break;
				case BY_TIME:
					condicio = getTimeFromBcn(d) > getTimeFromBcn(l->poi[i]->dest);
					break;
				default:
					//Si per un casual algú decideix afegir més camps, ens protegim ordenant segons entrada
					condicio = 0;
					break;
			}
			//Seguim saltant mentre no arribem al darrer element i es segueixi complint la condició de salt
		} while (condicio);

		//Apuntem els punters del nou node
		nou->prev[i] = l->poi[i]->prev[i];
		nou->next[i] = l->poi[i];
		//Fem que els elements als que hem apuntat ens apuntin a nosaltres
		nou->prev[i]->next[i] = nou;
		nou->next[i]->prev[i] = nou;
		//Deixem el pdi sobre el nou element
		l->poi[i] = nou;
	}
	l -> n++;
}

Destination readPoi(List l, int mode) {
	return l.poi[mode]->dest;
}

void removePoi(List * l, int mode) {
	// Comprovem que no estigui buida ni estiguem a un fantasma
	if (!isEmpty(*l) && l -> poi[mode] != l -> last && l -> poi[mode] != l -> first) {
		//Desem la posicio del node en memòria a un punter auxiliar, per no perdre'l
		Node * aux = l -> poi[mode];
		int i;
		//Deslliguem el node de la resta
		for (i = 0; i < MAX_SORTING; i++) {
			l -> poi[i] = aux -> next[i];

			aux -> prev[i] -> next[i] = aux -> next[i];
			aux -> next[i] -> prev[i] = aux -> prev[i];
		}
		l -> n--;
	}
}

void goStart(List * l, int mode) {
	l -> poi[mode] = l -> first -> next[mode];
}

void goEnd(List * l, int mode) {
	l -> poi[mode] = l -> last -> prev[mode];
}

void goNext(List * l, int mode) {
	l -> poi[mode] = l -> poi[mode] -> next[mode];
}

void goNextTimes(List * l, int mode, int times) {
	int i;
	for (i = 1; i < times; i++) {
		if (isEnd(*l, mode)) {
			break;
		}
		goNext(l, mode);
	}
}

void goPrev(List * l, int mode) {
	l -> poi[mode] = l -> poi[mode] -> prev[mode];
}

void goPrevTimes(List * l, int mode, int times) {
	int i;
	for (i = 1; i < times; i++) {
		if (isStart(*l, mode)) {
			break;
		}
		goPrev(l, mode);
	}
}

int isEnd(List l, int mode) {
	return l.poi[mode] == l.last;
}

int isStart(List l, int mode) {
	return l.poi[mode] == l.first;
}

int isEmpty(List l) {
	return *(l.first -> next) == l.last;
}

void destroy(List * l) {

	goStart(l, 0);

	while (!isEmpty(*l)) {
		removePoi(l, 0);
	}

}
