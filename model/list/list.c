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
	
}

Destination readPoi(List l, int mode) {
	
}

void removePoi(List * l, int mode) {
	
}

void goStart(List * l, int mode) {
	
}

void goEnd(List * l, int mode) {
	
}

void goNext(List * l, int mode) {
	
}

void goNextTimes(List * l, int mode, int times) {
	
}

void goPrev(List * l, int mode) {
	
}

void goPrevTimes(List * l, int mode, int times) {
	
}

int isEnd(List l, int mode) {
	
}

int isStart(List l, int mode) {
	
}

int isEmpty(List l) {
	
}

void destroy(List * l) {

}
