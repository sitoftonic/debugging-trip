#include "utils.h"
#include <stdlib.h>
#include <memory.h>


void resize(char ** pointer, int size) {
	char * new = (char *) malloc(sizeof(char) * size);

	memcpy(new, *pointer, sizeof(char) * size);
}