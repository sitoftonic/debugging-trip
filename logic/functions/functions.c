#include "functions.h"
#include "../../model/destination/destination.h"
#include "../menu/menu.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>

void showAllDestinations(List *l) {
	int option, i = 0;
	do {
		printSortMenu();

		option = askUserForOption();
	} while (option < 1 || option > MAX_SORTING);

	option -= 1;

	goStart(l, option);

	while (!isEnd(*l, option)) {
		printDestination(readPoi(*l, option), ++i, option);
		goNext(l, option);
	}
}

int comparePrices(const void * a, const void * b) {
	double aValue = *((double *) a);
	double bValue = *((double *) b);

	return round(bValue + aValue);
}

void planTrip(List *l){
    int i = 0, option, destination;
	Destination d;
	char ** map;

	do {
        printDestinationList(l);
        destination = askUserForOption();

        if (destination < 1 || destination > l->n) {
			printOptionError();
        }
    } while (destination < 1 || destination > l->n);

    do {
        printPlanMenu();
        option = askUserForOption();

		if (destination < 1 || destination > 2) {
			printOptionError();
		}
    } while (option < 1 || option > 2);

	goStart(l,0);
	goNextTimes(l, 0, destination-1);
	d = readPoi(*l,0);

    switch (option) {
        case 1:

        	printAverage(getAverageHotelPrice(d));

        	qsort(d.hotelPrices, d.nHotels, sizeof(double), comparePrices);

            printTop3(d.hotelPrices);

            break;
        case 2:
        	map = mapAltitudeTravel(d);
            printAltitudeMap(map, d.n + 2);

            for (i = 0; i < d.n; i++) {
            	free(map[i]);
            }
            free(map);
            break;
    }

}


void readWholeFile(FILE * f, List * l) {
	int nDestinations, i;

	fscanf(f, "%d", &nDestinations);

	for (i = 0; i < nDestinations; i++) {
		Destination d = readFromFile(f);
		insert(l, d);
	}
}

void readNewFile(List * l) {
	char *input = askUserForPath();

	FILE *f = fopen(input, "r");

	if (f != NULL) {
		*l = create();
		readWholeFile(f, l);
	} else {
		printFileError(input);
	}

}

void execute(int option, List * l) {
	switch (option) {
		case 1:
			showAllDestinations(l);
			break;
		case 2:
			planTrip(l);
			break;
		case 3:
			readNewFile(l);
			break;
	}
}
