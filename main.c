#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
// return number of bytes loaded if all went well
// -1 if it couldn't open the file
int loadProg(cell memory[MEMSIZE], char *path) {

	FILE* fp = fopen(path, "r");
	if (!fp) {
		printf("Failure to open file '%s'\n", path);
		return -1;
	}

	// load instructions (numbers separtated by spaces/newlines).

	int i = 0;
	char line[50];

	while (fgets(line, 50, fp) != NULL && i < MEMSIZE - 16) {
		sscanf(line, "%hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx",
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

// returns 0 if all went well
// 1 - invalid instruction encountered
// 2 - program should have ended
// if output is NULL, write to stdout
int run(cell memory[MEMSIZE], cell output[OUTPUTSIZE], int debug) {
	int ptr = 0;
	int outptr = 0;

	cell r0 = 0;
	cell r1 = 0;

	// used for swap
	cell tmp = 0;

	while (memory[ptr] != STOP) {
		if (ptr > MEMSIZE) {
			printf("Program should have ended. No more memory to read from\n");
			return 2;
		}
		// printf("%x %x\n", ptr, memory[ptr]);

		if (memory[ptr] == R0PP) { r0++; }
		else if (memory[ptr] == R0MM) { r0--; }
		else if (memory[ptr] == R1PP) { r1++; }
		else if (memory[ptr] == R1MM) { r1--; }
		else if (memory[ptr] == WRITE) {
			if (output != NULL) {
				output[outptr] = r0;
				outptr++;
			} else {
				printf("%c", r0);
			}
		} else if (memory[ptr] == PRINTDEC) {
			// write the number at the end of the output, making sure it doesn't
			// overflow
			if (output != NULL) {
				int written = snprintf(&output[outptr], OUTPUTSIZE - outptr, "%d", r0);
				outptr += written;
			} else {
				printf("%d", r0);
			}
		} else if (memory[ptr] == PRINTHEX) {
			// write the number at the end of the output, making sure it doesn't
			// overflow
			if (output != NULL) {
				int written = snprintf(&output[outptr], OUTPUTSIZE - outptr, "%x", r0);
				outptr += written;
			} else {
				printf("%x", r0);
			}
		} else if (memory[ptr] == PRINTBR) {
			// just a debug feature to quickly insert a line break in the ouput
			if (output != NULL) {
				output[outptr] = '\n';
				outptr++;
			} else {
				printf("\n");
			}
		// there is a -1 when we jump because the pointer is automatically
		// increased every loop
		} else if (memory[ptr] == JUMPEQ) {
			// watch out! random memory here!
			if (r0 == 0) {
				ptr = memory[ptr+1] - 1;
			} else {
				ptr++;
			}
		} else if (memory[ptr] == JUMPNEQ) {
			// watch out! random memory here!
			if (r0 != 0) {
				ptr = memory[ptr+1] - 1;
			} else {
				ptr++;
			}
		} else if (memory[ptr] == JUMP) {
			// watch out! random memory here!
			ptr = memory[ptr+1] - 1;
		} else if (memory[ptr] == SWAP0) {
			tmp = r0;
			// watch out! random memory here!
			ptr++;
			// this is nested because the instruction is:
			// swap <addr>. memory[ptr] is the address of the cell we should
			// swap with!
			r0 = memory[memory[ptr]];
			memory[memory[ptr]] = tmp;
		} else if (memory[ptr] == SWAP1) {
			tmp = r1;
			// watch out! random memory here!
			ptr++;
			r1 = memory[memory[ptr]];
			memory[memory[ptr]] = tmp;
		} else if (memory[ptr] == LOAD0) {
			ptr++;
			r0 = memory[ptr];
		} else if (memory[ptr] == LOAD1) {
			ptr++;
			r1 = memory[ptr];
		}
		else {
			printf("Invalid instruction 0x%x at 0x%x\n", memory[ptr], ptr);
			return 1;
		}
		ptr++;
		if (debug) 
			printf("ptr: 0x%x cell: 0x%x r0: %d r1: %d\n", ptr, memory[ptr], r0, r1);
	}

	return 0;
}


// returns 0 if it passes, otherwise a corresponding error code
// 1 - couldn't load program
// 2 - couldn't load expected output
// 3 - invalid instruction
// 4 - other runtime error
// >= 10: output differed from index code - 10
int test(char *name, cell memory[MEMSIZE], cell output[OUTPUTSIZE]) {

	if (DEBUG) 
		printf("Testing '%s'\n", name);

	// +1 for the null terminator
	char path[MAX_PATH_LENGTH + 1];

	snprintf(path, MAX_PATH_LENGTH+1, "./tests/%s.prg", name);

	int nloaded = loadProg(memory, path);
	if (nloaded == -1) {
		return 1;
	}
	int err = run(memory, output, DEBUG);
	if (err == 1) {
		return 3;
	} else if (err != 0) {
		return 4;
	}

	// ensure that the output corresponds to the content in the corresponding
	// file

	snprintf(path, MAX_PATH_LENGTH+1, "./tests/%s.out", name);

	FILE* fp = fopen(path, "r");
	if (!fp) {
		return 2;
	}

	int c;
	int i = 0;
	while ((c = fgetc(fp)) != EOF) {
		if (output[i] != c) {
			fclose(fp);
			return 10 + i;
		}
		i++;
	}

	if (ferror(fp)) {
		return 2;
	}

	// there is no more content in the file, ensure that the rest of the
	// program's output is still 0 as well
	while (i < OUTPUTSIZE) {
		if (output[i] != 0) {
			// the output differs at index i
			return 10 + i;
		}
		i++;
	}

	fclose(fp);
	return 0;
}

int runAllTests() {
	cell memory[MEMSIZE];
	cell output[OUTPUTSIZE];

	#define NUMBER_TESTS 5

	char *tests[NUMBER_TESTS] = {"2", "jump", "swap", "load", "print"};

	char failed = 0;
	for (int i = 0; i < NUMBER_TESTS; i++) {
		clearMemory(memory);
		clearOutput(output);
		int err = test(tests[i], memory, output);
		if (err == 1) {
			printf("Failure to load program for test '%s'\n", tests[i]);
		} else if (err == 2) {
			printf("Failure to load expected output for test '%s'\n", tests[i]);
		} else if (err == 3) {
			printf("Invalid instruction\n");
		} else if (err == 4) {
			printf("Runtime error\n");
		} else if (err >= 10) {
			printf("Outputs differ from index %d for test '%s'\n", err - 10, tests[i]);
			printf("Output from program:\n");
			printOutput(output);
		}
		if (err != 0) {
			failed++;
		}
	}
	if (!failed) {
		printf("All tests pass! You are awesome!\n");
	} else {
		printf("%d test(s) failed\n", failed);
	}
}

void showUsage() {
	printf("$ octrha <action>\n");
	printf("\n");
	printf("- help: show this help message\n");
	printf("- test: runs all the tests\n");
	printf("- other: assume it's a path to a file and run it\n");
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Wrong number of arguments: expect exaclty 1\n");
		showUsage();
	} else if (strcmp(argv[1], "help") == 0) {
		showUsage();
	} else if (strcmp(argv[1], "test") == 0) {
		runAllTests();
	} else {
		cell memory[MEMSIZE];

		clearMemory(memory);
		loadProg(memory, argv[1]);
		run(memory, NULL, 0);
	}

	return 0;
}

