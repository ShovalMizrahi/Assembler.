
/**************************************************************************
 *
 * WordMngr is responsible for adding the 24 bits words to the machine code.
 *
 * during first scan:
 * 1. checking if there are errors in the instruction
 * 2. adding the 24 bit word to the machine code.
 * 3. adding numbers in other words when necessary and leaving spaces for symbols.
 *
 * during second scan:
 * 1. checking if the symbols in the instructions are valid.
 * 2. adding the symbols in the suitable places left for them in the machine code.
 *
 **************************************************************************/




#ifndef ASSEMBLER_WORDMNGR_H
#define ASSEMBLER_WORDMNGR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "GlobalParams.h"
#include "SymbolMngr.h"
#define A 4
#define R 2
#define E 1
#define FALSE 0
#define TRUE 1
#define MAX_NUM_SIZE 1048575
#define MIN_NUM_SIZE (-1048576)
#define NO_REGISTER 0






enum opcodes {mov, cmp, add, sub, lea, clr, Not ,inc, dec, jmp , bne , jsr ,
red,prn,rts,stop};

enum methods{immediate, direct, relative, direct_reg};

enum  funct{zero,one,two,three,four};

/* being called to during the first scan. calls to other functions that create the 24 bit word and leaves spaces in the machine code for symbols */
int RunWordMngr(DATA data, MachineCode* code, int* C);
/* checks if there are errors in the instructions. returns true if there are no errors */
int checkError(DATA);
/* checks which opcode is used in the instruction */
void checkins(MachineCode *code, DATA data,int *c);
/* adds the opcode and funct to the 24 bit word */
void AddOpCodeAndFunct(MachineCode* code, int num,int *c);
/* adds the source register and it's method to the 24 bit word */
int AddSourceRegister(MachineCode* code, DATA data,int  * number,long * extra_num1, int * label,int * num_labels,int * c);
/* adds the destination register and it's method to the 24 bit word */
int AddDestinationRegister(MachineCode* code, DATA data,int  * number,long * extra_num2,int * label,int * num_labels,int * c);
/* If there is a number in the instruction, adds another word with the number to the machine code */
void AddExtraNumber(MachineCode* code, long num,int * c);
/* being called to during the second scan. adds the symbols to the machine code. */
int RunWordMngr_iter2(MachineCode* code, ptr* symbol_list, DATA data, int *c);
/* checks if the source is a register */
int CheckIfSourceRegister(DATA);
/* checks if the destination is a register */
int CheckIfDestinationRegister(DATA);


#endif

