#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "const.h"

void clearMemory(cell memory[MEMSIZE]) {
	for (int i = 0; i < MEMSIZE; i++) {
		memory[i] = 0;
	}
}

void clearOutput(cell output[OUTPUTSIZE]) {
	for (int i = 0; i < OUTPUTSIZE; i++) {
		output[i] = 0;
	}
}

void printOutput(cell output[OUTPUTSIZE]) {
	int i = 0;
	while (i < OUTPUTSIZE && output[i] != 0) {
		printf("%c", output[i]);
		i++;
	}
	printf("\n[%d character(s)]\n", i);
}

// loads a program into memory
// a program is made of lines of 16 instructions
int loadProg(cell memory[MEMSIZE], char *name) {

	// +1 for the null terminator
	char *path = malloc(sizeof(char) * MAX_PATH_LENGTH+1);

	snprintf(path, MAX_PATH_LENGTH+1, "./programs/%s.prg", name);

	FILE* fp = fopen(path, "r");
	if (!fp) {
		printf("Failure to open file '%s'\n", path);
		return -1;
	}
	free(path);

	// load instructions (numbers separtated by spaces/newlines).

	int i = 0;
	char line[50];

	while (fgets(line, 50, fp) != NULL) {
		sscanf(line, "%hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd",
			&memory[i+0], &memory[i+1], &memory[i+2], &memory[i+3],
			&memory[i+4], &memory[i+5], &memory[i+6], &memory[i+7],
			&memory[i+8], &memory[i+9], &memory[i+10], &memory[i+11],
			&memory[i+12], &memory[i+13], &memory[i+14], &memory[i+15]
		);
		i += 16;
	}

	if (ferror(fp)) {
		printf("I/O error while reading\n");
	}
	fclose(fp);
	return i * 16;

}

char run(cell memory[MEMSIZE], cell output[OUTPUTSIZE]) {
	int ptr = 0;
	int outptr = 0;

	cell r0 = 0;
	cell r1 = 0;

	// used for swap
	cell tmp = 0;
	printf("%d\n", memory[4] == 5);

	while (memory[ptr] != C_STOP) {
		if (memory[ptr] > INSTRUCTION_END) {
			printf("Invalid instruction %d at %x\n", memory[ptr], ptr);
			return 1;
		}
		printf("ptr: %d cell: %d r0: %d r1: %d\n", ptr, memory[ptr], r0, r1);
		if (memory[ptr] == R0PP) { r0++; }
		else if (memory[ptr] == R0MM) { r0--; }
		else if (memory[ptr] == R1PP) { r1++; }
		else if (memory[ptr] == R1MM) { r1--; }
		else if (memory[ptr] == PRINT) {
			output[outptr] = r0;
			outptr++;
		} else if (memory[ptr] == PRINTA) {
			// write the number at the end of the output, making sure it doesn't
			// overflow
			int written = snprintf(&output[outptr], OUTPUTSIZE - outptr, "%d", r0);
			outptr += written;
		} else if (memory[ptr] == JUMPEQ) {
			// watch out! random memory here!
			if (r0 == 0) {
				ptr = memory[ptr+1];
			} else {
				ptr++;
			}
		} else if (memory[ptr] == JUMPNEQ) {
			// watch out! random memory here!
			if (r0 != 0) {
				ptr = memory[ptr+1];
			} else {
				ptr++;
			}
		} else if (memory[ptr] == JUMP) {
			// watch out! random memory here!
			ptr = memory[ptr+1];
		} else if (memory[ptr] == SWAP0) {
			tmp = r0;
			// watch out! random memory here!
			r0 = memory[ptr+1];
			memory[ptr+1] = tmp;
		} else if (memory[ptr] == SWAP1) {
			tmp = r1;
			// watch out! random memory here!
			r1 = memory[ptr+1];
			memory[ptr+1] = tmp;
		}
		ptr++;
	}
}

// print memory array in rows of 16, in hexadecimal
void printMemory(cell memory[MEMSIZE]) {
	for (int i = 0; i < MEMSIZE; i++) {
		if ((i % 16) == 0) {
			printf("%3d| ", i);
		}
		printf("%2x ", memory[i]);
		if ((i % 16) == 15) {
			printf("\n");
		}
	}
}

int main(int argc, char *argv[]) {
	cell memory[MEMSIZE];
	cell output[OUTPUTSIZE];

	clearMemory(memory);
	clearOutput(output);

	int n = loadProg(memory, "hello");
	if (n == 0) {
		printf("Failure to load program into memory\n");
		return 0;
	} else {
		printf("Loaded %d byte(s) into memory\n", n);
	}

	printMemory(memory);
	run(memory, output);
	printOutput(output);

	return 0;
}

