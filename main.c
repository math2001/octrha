#include <stdio.h>

#include "const.h"
#include "debug.h"

void clearMemory(cell memory[MEMSIZE]) {
	for (int i = 0; i < MEMSIZE; i++) {
		memory[i] = 0;
	}
}

int main(int argc, char *argv[]) {
	cell memory[MEMSIZE];

	clearMemory(memory);
	dumpMemory(memory);
	return 0;
}

