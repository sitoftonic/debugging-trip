#ifndef TRIPORGANIZER_MENU_H
#define TRIPORGANIZER_MENU_H

#include "../../model/destination/destination.h"
#include "../../model/list/list.h"

void printWelcomeLine();
void printMenu();
void printOptionError();
void printFileError(char * file);
void printSortMenu();
void printDestination(Destination d, int pos, int option);
void printDestinationList(List * l);
void printPlanMenu();
void printAltitudeMap(char ** map, int n);
void printAverage(double average);
void printTop3(double * prices);
int askUserForOption();
int isCorrect(int option);
int isExit(int option);
char * askUserForPath();

#endif