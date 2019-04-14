#ifndef CONST_INCLUDED
#define CONST_INCLUDED

#define MEMSIZE 256
#define OUTPUTSIZE 256

#define MAX_CELL_DECIMAL_REPR 3
// this isn't actually true, but it's right for 99.9999% of the time
#define MAX_PATH_LENGTH 255

#define C_STOP 0
#define R0PP 1
#define R0MM 2
#define R1PP 3
#define R1MM 4
#define PRINT 5
#define PRINTA 6

#define JUMPEQ 7
#define JUMPNEQ 8
#define JUMP 9
#define SWAP0 10
#define SWAP1 11

#define INSTRUCTION_END SWAP1

#define NUMBER_TESTS 1

#define DEBUG 0

typedef unsigned char cell;

#endif