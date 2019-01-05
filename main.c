#include <stdio.h>
#include <stdlib.h>
#include "logic/menu/menu.h"
#include "logic/functions/functions.h"

#define PATH "data/mainDatabase.txt"

int main() {
	int option;
	// Obrim fitxer que conté les dades
	FILE* f = fopen(PATH, "r");
	// Si no s'ha pogut obrir el fitxer, s'acaba la execució del programa
	if (f == NULL) {
		printFileError(PATH);
		return -1;
	}
	// Es crea la llista
	List l = create();
	// Es llegeix tota la informació del fitxer i s'emmagatzema dins la llista
	readWholeFile(f, &l);
	// Es tanca el fitxer
    fclose(f);
	// Comença la interfície gràfica
	printWelcomeLine();
	// S'executa el programa mentre l'usuari no surti
	do {
		printMenu();
		// Mentre l'opció escollida sigui incorrecta
		do {
			// Es llegeix la opció escollida per l'usuari
			option = askUserForOption();

			if (!isCorrect(option)) {
				printOptionError();
			}
		} while (!isCorrect(option));
		// S'executa la opció escollida
		execute(option, &l);
	} while (!isExit(option));
	// Es destrueix la llista
	destroy(&l);

	return 0;
}
