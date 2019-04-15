#ifndef CONST_INCLUDED
#define CONST_INCLUDED

#define MEMSIZE 256
#define OUTPUTSIZE 256

#define MAX_CELL_DECIMAL_REPR 3
// this isn't actually true, but it's right for 99.9999% of the time
#define MAX_PATH_LENGTH 255

#define STOP       0x0
#define R0PP       0x1
#define R0MM       0x2
#define R1PP       0x3
#define R1MM       0x4
#define WRITE      0x5
#define PRINTASCII 0x6

#define JUMPEQ     0x20
#define JUMPNEQ    0x21
#define JUMP       0x22
#define SWAP0      0x23
#define SWAP1      0x24
#define LOAD0      0x25
#define LOAD1      0x26

#define LINEBREAK  0xFF

#define NUMBER_TESTS 4

#define DEBUG 1

typedef unsigned char cell;

#endif