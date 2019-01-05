#include "functions.h"
#include "../menu/menu.h"
#include <stdlib.h>
#include <math.h>

// Funció que printa totes les destinacions
void showAllDestinations(List *l) {
	int option, i = 0;
	// Printem el menu fins que la opció sigui correcta
	do {
		printSortMenu();
		// Demanem la opció a l'usuari
		option = askUserForOption();
	} while (option < 1 || option > MAX_SORTING);
	// Restem una posició a la opció ja que aquesta defineix la posició d'un array
	option -= 1;
	// Es col·loca el POI a la primera posició de la llista ordenada per option
	goStart(l, option);
	// Mentre no s'acaba la llista es printa la informació de la destinació
	while (!isEnd(*l, option)) {
		printDestination(readPoi(*l, option), ++i, option);
		goNext(l, option);
	}
}

// Funció que compara els preus de les dues destinacions a i b
int comparePrices(const void * a, const void * b) {
	double aValue = *((double *) a);
	double bValue = *((double *) b);

	return (int)round(bValue - aValue);
}

// Funció que s'encarrega de planificar un viatge
void planTrip(List *l){
    int i = 0, option, destination;
	Destination d;
	char ** map;
	// Es printa la llista de destinacions fins que l'usuari introdueixi una opció vàlida
	do {
        printDestinationList(l);
		// Demanem opció a l'usuari
        destination = askUserForOption();

        if (destination < 1 || destination > l->n) {
			printOptionError();
        }
    } while (destination < 1 || destination > l->n);
	// Es printen les opcions mentre l'opció introduida per l'usuari sigui vàlida
    do {
        printPlanMenu();
		// Demanem opció a l'usuari
        option = askUserForOption();

		if (option < 1 || option > 2) {
			printOptionError();
		}
    } while (option < 1 || option > 2);
	// Col·loquem el POI de la llista al principi
	goStart(l,0);
	// Avancem per la llista fins arribar a la destinació
	goNextTimes(l, 0, destination);
	// Llegim la destinació
	d = readPoi(*l,0);

	// Escollim funcionalitat en funció de la opció introduida per l'usuari
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
		default:
			break;
    }

}

// Funció que llegeix tota la informació del fitxer f i la introdueix a la llista
void readWholeFile(FILE * f, List * l) {
	int nDestinations, i;

	fscanf(f, "%d", &nDestinations);

	for (i = 0; i < nDestinations; i++) {
		Destination d = readFromFile(f);
		insert(l, d);
	}
}

// Funció que llegeix tota la informació d'un fitxer nou, borra la informació de la llista
// i li introdueix la nova informació
void readNewFile(List * l) {
	char *input = askUserForPath();

	FILE *f = fopen(input, "r");

	if (f != NULL) {
		*l = create();
		readWholeFile(f, l);
		fclose(f);
	} else {
		printFileError(input);
	}

}

// Funció que executa les funcionalitats del programa
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
		default:
			break;
	}
}
