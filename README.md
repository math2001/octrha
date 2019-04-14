# OCTRHA

> One Chip To Rule tHem All

## Instruction set

| ID | Name  | Comment |
| 0  | Abort | Exit program|
| 1  | R0++  | Increment register 0 |
| 2  | R0--  | Decrement register 0 |
| 3  | R1++  | Increment register 1 |
| 4  | R1--  | Decrement register 1 |
| 5  | JumpEq | Increment register 1 |
| 6  |   | Decrement register 1 |

*not done*

## Format

> the format is probably going to change, right now, it's just copying the
> course's chip "word for word".

The program in the .prg file must represent the whole memory of the chip (if
it isn't specified, the rest is filled in with 0).

It must be rows of 16 instructions seperated by spaces, in hexadecimal.

You have 256 bytes of memory, therefore, you should have at most 16 rows of
instructions.

## Tips

1. Make your life easy and use Linux line feed for .out files
2. when you want to know the address of a cell, because the file has to be 16x16, the first digit is the row number (starting from 0 of course), and the
second the column (starting from 0 as well).