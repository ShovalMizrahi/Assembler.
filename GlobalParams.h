#ifndef ASSEMBLER_GLOBAL_PARAMS_H
#define ASSEMBLER_GLOBAL_PARAMS_H
/*********************************************************************************************************
 *
 * Global Params file
 *
 *********************************************************************************************************/

#define TRUE 1
#define FALSE 0
#define ARR_SIZE 255
#define MAX_LINES_IN_CODE 255
#define MAX_LINE_LENGTH 80
#define SHORT_ARR_SIZE 31
#define MAX_LBL_SIZE 32
#define IC_DEFAULT_VALUE 100
#define DC_DEFAULT_VALUE 0
#define NUM_OPCODES 16
#define NUM_BITS_IN_WORD 24

#define OPCODES {"mov", "cmp", "add", "sub", "lea","clr","not","inc","dec","jmp","bne","jsr",\
"red","prn","rts","stop"}

#define REGISTERS {"r0","r1","r2","r3","r4","r5","r6", "r7"}
#define NUM_REGISTERS 8

#define SAVED_WORDS {"entry", "extern", "string", "data", "PSW"};
#define NUM_SAVED_WORDS 5

/*
 * typedef Data - which contains all the data on a single line, while being parsed this struct is filled.
 */
typedef struct {
    unsigned int hasSymbol;
    unsigned int isData;
    unsigned int isString;
    unsigned int isInstruction;
    unsigned int isExtern;
    unsigned int isEntry;
    int numWords;
    long *Data;
    int size_data;
    char Symbol[MAX_LBL_SIZE];
    char opCode[SHORT_ARR_SIZE];
    char source[MAX_LBL_SIZE];
    char destination[MAX_LBL_SIZE];
    char string[ARR_SIZE];
} DATA;

/*
 * Symbol list, defined as a linked-list
 */
typedef struct symbol_list *ptr;

typedef struct symbol_list {
    unsigned int memory_address;
    unsigned int isExtern;
    unsigned int isEntry;
    unsigned int isCode;
    char name[MAX_LBL_SIZE];
    ptr next;
} item;

int line_index;

/* enum defines options for read line result (another is EOF, meaning end of file, which is default) */
enum nextLine {
    skip_next_line_no_error, skip_next_line_error, do_next_line
};

/* struct for the 24 bit words in the machine code */
struct Word {
    unsigned int ARE: 3;
    unsigned int funct: 5;
    unsigned int reg_des: 3;
    unsigned int method_des: 2;
    unsigned int reg_src: 3;
    unsigned int method_src: 2;
    unsigned int opcode: 6;
};

/* easy access to the decimal value of the 24 bit words */
union number {
    long num; /* using long because on certain systems int may be smaller than 24 bits */
    struct Word this_word;
};

/* struct for the machine code that stores the 24 bit words */
typedef struct {
    int address;
    char name[MAX_LBL_SIZE];
    union number line;
} MachineCode;


#endif
