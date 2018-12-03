#ifndef TRIPORGANIZER_LIST_H
#define TRIPORGANIZER_LIST_H

#include "../destination/destination.h"

#define MAX_SORTING 4

typedef enum {
	BY_NAME,
	BY_AVG_PRICE,
	BY_DISTANCE,
	BY_TIME
} Sorting;

typedef struct _Node {
	Destination dest;
	struct _Node * next[MAX_SORTING];
	struct _Node * prev[MAX_SORTING];
} Node;

typedef struct {
	Node * first;
	Node * last;
	Node * poi[MAX_SORTING];
	int n;
} List;

List create();

void insert(List * l, Destination d);

Destination readPoi(List l, int mode);

void removePoi(List * l, int mode);

void goStart(List * l, int mode);

void goEnd(List * l, int mode);

void goNext(List * l, int mode);

void goNextTimes(List * l, int mode, int times);

void goPrev(List * l, int mode);

void goPrevTimes(List * l, int mode, int times);

int isEnd(List l, int mode);

int isStart(List l, int mode);

int isEmpty(List l);

void destroy(List * l);

#endif
