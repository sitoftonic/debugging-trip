#include <stdio.h>
#include <stdlib.h>
#include "logic/menu/menu.h"
#include "logic/functions/functions.h"
#include "model/list/list.h"
#include "model/destination/destination.h"

#define PATH "data/mainDatabase.nymal"

int main() {
	int option;

	FILE* f = fopen(PATH, "r");

	if (f == NULL) {
		printFileError(PATH);
		return -1;
	}

	List l = create();

	readWholeFile(f, &l);


	printWelcomeLine();

	do {
		printMenu();

		do {
			option = askUserForOption();

			if (!isCorrect(option)) {
				printOptionError();
			}
		} while (!isCorrect(option));

		execute(option, &l);
	} while (!isExit(option));

	destroy(&l);

	return 0;
}
