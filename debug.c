#include <stdio.h>

#include "const.h"
#include "debug.h"

// print memory array in rows of 16, in hexadecimal
void dumpMemory(cell memory[MEMSIZE]) {
	for (int i = 0; i < MEMSIZE; i++) {
		if ((i % 16) == 0) {
			printf("%2d| ", i / 16);
		}
		printf("%2x ", memory[i]);
		if ((i % 16) == 15) {
			printf("\n");
		}
	}
}