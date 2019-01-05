#include "utils.h"
#include <stdlib.h>
#include <memory.h>

// Funció que canvia la mida d'una string i la fa més gran
void resize(char* pointer, int size) {
	char * new = (char *) malloc(sizeof(char) * size);

	memcpy(new, pointer, sizeof(char) * size);
}