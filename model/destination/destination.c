#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include "destination.h"
#include "../../logic/utils/utils.h"

#define toRadians(degrees) ((degrees) * M_PI / 180.0)

#define TIME_FACTOR 3.7

#define DIRECTIONS 4
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

double haversine(double ang) {
	return pow(sin(ang / 2), 2.0);
}

// Distància haversine des de BCN
double getDistanceFromBcn(Destination dest) {
	Destination bcn;
	bcn.location.lat = BCN_LON;
	bcn.location.lon = BCN_LAT;

	return getDistanceBetween(bcn, dest);
}

// Distància haversine
double getDistanceBetween(Destination this, Destination that) {
	double latA = toRadians(this.location.lat);
	double latB = toRadians(that.location.lat);
	double lonA = toRadians(this.location.lon);
	double lonB = toRadians(that.location.lon);

	double angleCentral = haversine(latA - latB);
	angleCentral += cos(latA) * cos(latB) * haversine(lonA - lonB);
	angleCentral = 2 * asin(sqrt(angleCentral));

	return EARTH_RADIUS * angleCentral;
}

// Retorna la mitjana de preus d'hotels
double getAverageHotelPrice(Destination dest) {
	double average = 0.0;
	int i;

	for (i = 1; i < dest.nHotels; i++) {
		average += *(dest.hotelPrices + i) / dest.nHotels;
	}

	return average;
}

// Modifica coordenades segons la direcció
void advanceInDirection(int *i, int *j, int direction) {
	switch (direction) {
		case NORTH:
			(*j)--;
			break;
		case EAST:
			(*i)++;
			break;
		case SOUTH:
			(*j)++;
			break;
		case WEST:
		default:
			(*i)--;
			break;
	}
}

// Retorna l'altitud de la casella en una direcció de l'actual o INF si hi ha error
double checkAltitudeInDirection(double ** travelAltitude, int i, int j, int direction, int n) {
	advanceInDirection(&i, &j, direction);

	if (i >= n || j >= n || i < 0 || j < 0) {
		return INFINITY;
	}
	return travelAltitude[i][j];
}

// Recorre recursivament la matriu buscant la mínima altitud i retornant el total recorregut
// Podeu assumir que les dades sempre donaran un camí decreixent en altitud de 0, 0  a n-1, n-1
double travelThroughAltitude(double ** travelAltitude, int i, int j, int n) {
	if (i == n - 1 && j == n - 1) {
		return travelAltitude[i][j];
	}

	int newI = i, newJ = j, min = NORTH;
	double minAlt = 0;

	for (int k = 0; k < DIRECTIONS; k++) {
		double alt = checkAltitudeInDirection(travelAltitude, i, j, k, n);

		if (alt < minAlt) {
			minAlt = alt;
			min = k;
		}
	}

	advanceInDirection(&newI, &newJ, min);

	return travelAltitude[i][j] + travelThroughAltitude(travelAltitude, i, j, n);
}

// Retorna una estimació del temps de viatge des de BCN segons l'altitud
double getTimeFromBcn(Destination dest) {
	double time = getDistanceFromBcn(dest) / TIME_FACTOR;

	time += travelThroughAltitude(dest.travelAltitude, 0, 0, dest.n);

	return time;
}

// Recorre recursivament la matriu buscant la mínima altitud i mapejant el recorregut
// Podeu assumir que les dades sempre donaran un camí decreixent en altitud de 0, 0  a n-1, n-1
void mapAltitude(double ** travelAltitude, char ** map, int i, int j, int n) {
	map[i + 1][j + 1] = ' ';

	if (i == n - 1 && j == n - 1) {
		return;
	}

	int newI = i, newJ = j, min = NORTH;
	double minAlt = 0;

	for (int k = 0; k < DIRECTIONS; k++) {
		double alt = checkAltitudeInDirection(travelAltitude, i, j, k, n);

		if (alt < minAlt) {
			minAlt = alt;
			min = k;
		}
	}

	advanceInDirection(&newI, &newJ, min);

	mapAltitude(travelAltitude, map, i, j, n);
}


// Retorna un mapeig del recorregut segons l'altitud mínima, amb un marc
char ** mapAltitudeTravel(Destination dest) {
	int i, j;
	char ** map = (char **)malloc(sizeof(char *) * (dest.n + 2));

	for (i = 0; i < dest.n + 2; i++) {
		map[i] = (char *) malloc(sizeof(char) * (dest.n + 2));

		for (j = 0; j < dest.n + 2; j++) {
			map[i][j] = '#';
		}
	}

	mapAltitude(dest.travelAltitude, map, 0, 0, dest.n);

	return map;
}

// Allibera tota la memòria d'una destinació
void freeAll(Destination dest) {
	free(dest.name);
	free(dest.country);
	free(dest.hotelPrices);
	free(dest.travelAltitude);
}

char * readString(FILE * file) {
	int i = 0;
	int size = 10;
	char tmp;
	char* ret = (char*) malloc(sizeof(char) * size);


	fscanf(file, "%c", &tmp);
	do {
		i++[ret] = tmp;

		if (i >= size) {
			size *= 2;
			resize(&ret, size);
		}
		fscanf(file, "%c", &tmp);
	} while(tmp != '\n');

	resize(&ret, i + 1);


	return ret;
}

Destination readFromFile(FILE * file) {
	Destination d;
	int i = 0, j = 0;

	char dummy;
	fscanf(file, "%c", &dummy);

	d.name = readString(file);
	d.country = readString(file);

	fscanf(file, "%d", &d.area);
	d.n = (int) sqrt(d.area);

	d.travelAltitude = (double**) malloc(sizeof(double*) * d.n);

	for (i = 0;  i < d.n ; i++) {
		d.travelAltitude[i] = (double*) malloc(sizeof(double) * d.n);
		for (j = 0;  j < d.n ; j++) {
			fscanf(file, "%lf", &d.travelAltitude[i][j]);
		}
	}

	fscanf(file, "%d", &d.nHotels);

	d.hotelPrices = (double*) malloc(sizeof(double) * d.nHotels);
	for (i = 0;  i < d.nHotels; i++) {
		fscanf(file, "%lf", &d.hotelPrices[i]);
	}

	fscanf(file, "%lf", &d.location.lat);
	fscanf(file, "%lf", &d.location.lon);

	return d;
}