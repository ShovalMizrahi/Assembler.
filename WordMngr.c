#include "WordMngr.h"

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

/* being called to during the first scan. calls to other functions that create the 24 bit word and leaves spaces in the machine code for symbols */
int RunWordMngr(DATA data, MachineCode* code, int* C)
{
    long extra_num1=0, extra_num2=0; /* numbers that need another word in the machine code */
    int number=FALSE; /* boolean value. true if there is a number in the instruction. false otherwise */
    int label=FALSE; /* boolean value. true if there is a symbol in the instruction. false otherwise */
    int num_labels=0; /* numbers of labels in the instruction */
    if(checkError(data) == TRUE) {
        checkins(code, data, C);
        AddSourceRegister(code, data, &number, &extra_num1, &label, &num_labels, C);
        AddDestinationRegister(code, data, &number, &extra_num2, &label, &num_labels, C);
        code[*C].address = *C + IC_DEFAULT_VALUE; /* the address of the word in the machine code */
        (*C)++;
        if (label == TRUE) {
            if (number == TRUE)
            {
                /*the number is the source and the symbol is the destination. */
                if (data.source[0] == '#')
                {
                    AddExtraNumber(code, extra_num1, C);
                    code[(*C)].address = (*C) + IC_DEFAULT_VALUE;
                    (*C)++;
                    code[(*C)].line.this_word.ARE = 0; /*during second iteration we will check if A,R AND E =0,
                                                    if they do that means this place is empty. */
                    code[(*C)].address = (*C) + IC_DEFAULT_VALUE;
                    (*C)++;
                }
                /* the symbol is the source and the number is the destination. */
                else
                {
                    code[(*C)].line.this_word.ARE = 0; /*during second iteration we will check if A,R AND E =0,
                                                    if they do that means this place is empty. */
                    code[(*C)].address = (*C) + IC_DEFAULT_VALUE;
                    (*C)++;
                    AddExtraNumber(code, extra_num2, C);
                    code[(*C)].address = (*C) + IC_DEFAULT_VALUE;
                    (*C)++;
                }
            }
            else
            {
                while (num_labels != 0)
                {
                    code[(*C)].line.this_word.ARE = 0; /*during second iteration we will check if A,R AND E =0,
                                                if they do that means this place is empty. */
                    code[(*C)].address = (*C) + IC_DEFAULT_VALUE;
                    (*C)++;
                    num_labels--;
                }
            }
            return TRUE;
        }
        if (number == TRUE)
        {
            /*checks if the source is a number */
            if (data.source[0] == '#')
            {
                AddExtraNumber(code, extra_num1, C);
                code[(*C)].address = (*C) + IC_DEFAULT_VALUE;
                (*C)++;
            }
            /*checks if the destination is a number */
            if (data.destination[0] == '#')
            {
                AddExtraNumber(code, extra_num2, C);
                code[(*C)].address = (*C) + IC_DEFAULT_VALUE;
                (*C)++;
            }
        }
        return TRUE;
    }
    else
        return FALSE;
}

/* checks which opcode is used in the instruction */
void checkins(MachineCode *code, DATA data,int *c)
{
    int i;
    char * OpCodes[] = OPCODES;
    for(i=0;i<NUM_OPCODES;i++)
    {
        if(strcmp(OpCodes[i],data.opCode) == 0)
        {
            AddOpCodeAndFunct(code,i,c);
            break;
        }
    }
}

/* adds the opcode and funct to the 24 bit word */
void AddOpCodeAndFunct(MachineCode* code, int num,int *c)
{
    code[*c].line.this_word.ARE = A;
    if(num == mov || num == cmp || num == lea || num >= red)
    {
        code[*c].line.this_word.opcode = num;
        code[*c].line.this_word.funct = zero;
    }
    if(num == add || num == clr || num == jmp)
    {
        code[*c].line.this_word.opcode = num;
        code[*c].line.this_word.funct = one;
    }
    if(num == sub || num == Not || num == bne)
    {
        code[*c].line.this_word.opcode = num-1;
        code[*c].line.this_word.funct = two;
    }
    if(num == inc || num == jsr)
    {
        code[*c].line.this_word.opcode = num-2;
        code[*c].line.this_word.funct = three;
    }
    if(num == dec)
    {
        code[*c].line.this_word.opcode = num-3;
        code[*c].line.this_word.funct = four;
    }

}

/* adds the source register and it's method to the 24 bit word */
int AddSourceRegister(MachineCode* code, DATA data,int  * number,long * extra_num1, int * label,int * num_labels,int * c)
{
    if(data.source[0] != '\0')
    {
        int i;
        char * registers[] = REGISTERS;
        for(i = 0;i<8;i++)
        {
            /* if the source is a register, adds the number of the register to the source in the 24 bit word */
            if (strcmp(registers[i], data.source) == 0)
            {
                code[*c].line.this_word.reg_src = i;
                code[*c].line.this_word.method_src = direct_reg;
                return TRUE;
            }
        }

        if(data.source[0] == '#')
        {
            int j;
            char * eptr; /* used for strtol function */
            char num_str[9]; /* highest/lowest value can be 1,048,575/-1,048,576 */
            code[*c].line.this_word.reg_src = NO_REGISTER;
            code[*c].line.this_word.method_src = immediate;
            for(j = 1;data.source[j] != '\0';j++)
            {
                num_str[j-1] = data.source[j];
            }
            *extra_num1 = strtol(num_str,&eptr,10);
            *number = TRUE;
            return TRUE;
        }
        else /* is a label */
        {
            code[*c].line.this_word.reg_src = NO_REGISTER;
            code[*c].line.this_word.method_src = direct;
            *label = TRUE;
            (*num_labels)++;
            return  TRUE;
        }
    }
    return TRUE;
}

/* adds the destination register and it's method to the 24 bit word */
int AddDestinationRegister(MachineCode* code, DATA data,int  * number,long * extra_num2,int * label,int * num_labels,int * c)
{
    if (data.destination[0])
    {
        int i;
        char *registers[] = REGISTERS;
        for (i = 0; i < 8; i++)
        {
            if (strcmp(data.destination, registers[i]) == 0)
            {
                code[*c].line.this_word.reg_des = i;
                code[*c].line.this_word.method_des = direct_reg;
                return TRUE;
            }
        }
        if (data.destination[0] == '#')
        {
            char * eptr; /* used for strtol function */
            char num_str[9]; /* highest/lowest value can be 1,048,575/-1,048,576 */
            code[*c].line.this_word.reg_des = NO_REGISTER;
            code[*c].line.this_word.method_des = immediate;
            for (i = 1; data.destination[i] != '\0'; i++)
            {
                num_str[i - 1] = data.destination[i];
            }
            *extra_num2 = strtol(num_str,&eptr,10);
            *number = TRUE;
            return TRUE;
        }
        /* destination is a label used in relative method */
        if (data.destination[0] == '&')
        {
            code[*c].line.this_word.reg_des = NO_REGISTER;
            code[*c].line.this_word.method_des = relative;
            *label = TRUE;
            (*num_labels)++;
            return TRUE;
        }
        else /* also label just not with & */
        {
            code[*c].line.this_word.reg_des = NO_REGISTER;
            code[*c].line.this_word.method_des = direct;
            *label = TRUE;
            (*num_labels)++;
            return TRUE;
        }
    }
    return TRUE;
}

/* If there is a number in the instruction, adds another word with the number to the machine code */
void AddExtraNumber(MachineCode* code, long extra_num,int * c)
{
    union number n;
    /* moving thr number 3 bits to the right because the first 3 are A \ R \ E */
    n.num = (extra_num << 3);
    code[*c].line.this_word.ARE = A;
    code[*c].line.this_word.funct = n.this_word.funct;
    code[*c].line.this_word.reg_des = n.this_word.reg_des;
    code[*c].line.this_word.method_des = n.this_word.method_des;
    code[*c].line.this_word.reg_src = n.this_word.reg_src;
    code[*c].line.this_word.method_src = n.this_word.method_src;
    code[*c].line.this_word.opcode = n.this_word.opcode;
}

/* checks if there are errors in the instructions. returns true if there are no errors */
int checkError(DATA data)
{
    char * opCodes[] = OPCODES;
    if(data.source[0] == '#')
    {
        int i;
        long num;
        char * eptr; /* used for the strtol function */
        char str_num[SHORT_ARR_SIZE];
        /* checks if number's syntax is valid */
        if(data.source[1] == '-' || data.source[1] == '+')
        {
            int j;
            /* checks that the number contains only digits */
            for (j=2;data.source[j];j++)
            {
                if(isdigit(data.source[j]) == 0)
                {
                    fprintf(stdout,"%d: Error: %s isn't a valid number.\n",line_index,data.source);
                    return  FALSE;
                }
            }
        }
        else
        {
            int j;
            /* checks that the number contains only digits */
            for (j=1;data.source[j];j++)
            {
                if(isdigit(data.source[j]) == 0)
                {
                    fprintf(stdout,"%d: Error: %s isn't a valid number.\n",line_index,data.source);
                    return  FALSE;
                }
            }
        }
        for (i = 1;data.source[i];i++)
        {
            str_num[i-1] = data.source[i];
        }
        num = strtol(str_num,&eptr,10);
        /* checks if number exceeds 21 bits */
        if( num > MAX_NUM_SIZE || num < MIN_NUM_SIZE)
        {
            fprintf(stdout,"%d: Error: size of number %s exceeds 21 bits.\n",line_index,data.source);
            return FALSE;
        }
    }
    if(data.destination[0] == '#')
    {
        int i;
        long num;
        char * eptr; /* used for strtol function */
        char str_num[SHORT_ARR_SIZE];
        /* checks if number's syntax is valid */
        if(data.destination[1] == '-' || data.destination[1] == '+')
        {
            int j;
            /* checks that the number contains only digits */
            for (j=2;data.destination[j];j++)
            {
                if(isdigit(data.destination[j]) == 0)
                {
                    fprintf(stdout,"%d: Error: %s isn't a valid number.\n",line_index,data.destination);
                    return  FALSE;
                }
            }
        }
        else
        {
            int j;
            /* checks that the number contains only digits */
            for (j=1;data.destination[j];j++)
            {
                if(isdigit(data.destination[j]) == 0)
                {
                    fprintf(stdout,"%d: Error: %s isn't a valid number.\n",line_index,data.destination);
                    return  FALSE;
                }
            }
        }
        for (i = 1;data.destination[i];i++)
        {
            str_num[i-1] = data.destination[i];
        }
        num = strtol(str_num,&eptr,10);
        /* checks if number exceeds 21 bits */
        if( num > MAX_NUM_SIZE || num < MIN_NUM_SIZE)
        {
            fprintf(stdout,"%d: Error: size of number %s exceeds 21 bits.\n",line_index, data.destination);
            return FALSE;
        }
    }
    /* checks if there is a # sign before number */
    if (isdigit(data.source[0]) > 0 || isdigit(data.destination[0]) > 0)
    {
        fprintf(stdout,"%d: Error: number has to come after # sign.\n",line_index);
        return FALSE;
    }

    if (strcmp(data.opCode,opCodes[mov]) == 0 || strcmp(data.opCode,opCodes[cmp]) == 0 || strcmp(data.opCode,opCodes[add]) == 0
            || strcmp(data.opCode,opCodes[sub]) == 0 || strcmp(data.opCode,opCodes[lea]) == 0) {
        /* checking that the instructions have the right amount of operands */
        if (data.source[0] == '\0' || data.destination[0] == '\0')
        {
            fprintf(stdout,"%d: Error: instruction %s must have two operands.\n",line_index,data.opCode);
            return FALSE;
        }
        else
        {
            /* checking if the operands are valid */
            if(strcmp(data.opCode,opCodes[lea]) == 0)
            {
                if (data.source[0] == '#')
                {
                    fprintf(stdout, "%d: Error: instruction %s cant have a number as a source.\n", line_index,
                            data.opCode);
                    return FALSE;
                }
                if (CheckIfSourceRegister(data))
                {
                    fprintf(stdout, "%d: Error: instruction %s can't have a register as an source.\n", line_index,
                            data.opCode);
                    return FALSE;
                }
            }
            /* checking if the operands are valid */
            if(data.destination[0] == '#')
            {
                if (strcmp(data.opCode,opCodes[cmp]) != 0)
                {
                    fprintf(stdout, "%d: Error: instruction %s can't have a number as a destination.\n",line_index,data.opCode);
                    return FALSE;
                }
            }
            if (data.source[0] == '&' || data.destination[0] == '&')
            {
                fprintf(stdout, "%d: Error: instruction %s can't use & sign.\n",line_index,data.opCode);
                return FALSE;
            }
            else
                return  TRUE;
        }
    }
    if (strcmp(data.opCode,opCodes[clr]) == 0 || strcmp(data.opCode,opCodes[Not]) == 0 || strcmp(data.opCode,opCodes[inc]) == 0
        || strcmp(data.opCode,opCodes[dec]) == 0 || strcmp(data.opCode,opCodes[red]) == 0 || strcmp(data.opCode,opCodes[prn]) == 0)
    {
        /* checking that the instructions have the right amount of operands */
        if(data.source[0] != '\0')
        {
            fprintf(stdout,"%d: Error: instruction %s can only have one operand.\n",line_index,data.opCode);
            return  FALSE;
        }
        else
        {
            if(data.destination[0] == '\0')
            {
                fprintf(stdout,"%d: Error: instruction %s must have an operand.\n",line_index,data.opCode);
                return FALSE;
            }
            /* checking if the operands are valid */
            if(data.destination[0] == '#')
            {
                if (strcmp(data.opCode,opCodes[prn]) != 0)
                {
                    fprintf(stdout, "%d: Error: instruction %s can't have a number as an operand.\n",line_index,data.opCode);
                    return FALSE;
                }
            }
            if (data.destination[0] == '&')
            {
                fprintf(stdout, "%d: Error: instruction %s can't use & sign.\n",line_index,data.opCode);
                return FALSE;
            }
            else
                return  TRUE;
        }
    }
    if (strcmp(data.opCode,opCodes[jmp]) == 0 || strcmp(data.opCode,opCodes[bne]) == 0 || strcmp(data.opCode,opCodes[jsr]) == 0)
    {
        /* checking that the instructions have the right amount of operands */
        if(data.source[0] != '\0')
        {
            fprintf(stdout,"%d: Error: instruction %s can only have one operand.\n",line_index,data.opCode);
            return FALSE;
        }
        else
        {
            if(data.destination[0] == '\0')
            {
                fprintf(stdout,"%d: Error: instruction %s must have an operand.\n",line_index,data.opCode);
                return FALSE;
            }
            /* checking if the operands are valid */
            if (data.destination[0] == '#')
            {
                fprintf(stdout, "%d: Error: instruction %s can't have a number as an operand.\n",line_index,data.opCode);
                return FALSE;
            }
            if(CheckIfDestinationRegister(data))
            {
                fprintf(stdout, "%d: Error: instruction %s can't have a register as an operand.\n",line_index,data.opCode);
                return FALSE;
            }
            return TRUE;
        }
    }

    if(strcmp(data.opCode,opCodes[rts]) == 0 || strcmp(data.opCode,opCodes[stop]) == 0)
    {
        /* checking that the instructions have the right amount of operands */
        if(data.source[0] != '\0' || data.destination[0] != '\0')
        {
            fprintf(stdout,"%d: Error: instruction %s must have zero operands.\n",line_index,data.opCode);
            return  FALSE;
        }
        else
            return TRUE;

    }
    return TRUE;
}

/* being called to during the second scan. adds the symbols to the machine code. */
int RunWordMngr_iter2(MachineCode* code, ptr* symbol_list, DATA data, int *C)
{
    int second_word = FALSE;
    int no_error = TRUE;
    (*C)++;
    if (data.source[0] != '\0')
    {
        /* checks if source is a number */
        if (data.source[0] != '#')
        {
            /* checks if source is a register */
            int tmp = CheckIfSourceRegister(data);
            if (tmp == FALSE)
            {
                /* getting the symbol from the symbols list */
                ptr symbol = getAddressOfSymbol(symbol_list, data.source);
                /* checking if the symbol exists */
                if (symbol == NULL)
                {
                    fprintf(stdout, "%d: Error: Symbol %s was not found on the symbol table.\n", line_index,data.source);
                    (*C)++;
                    no_error = FALSE;
                }
                else
                {
                    if (symbol->isExtern)
                    {
                        (*C)++;
                        code[(*C)].line.this_word.ARE = E;
                        strcpy(code[(*C)].name, symbol->name);

                    }
                    else
                    {
                        (*C)++;
                        /* memory address is at max 21 bits and words are 24 bits. */
                        code[(*C)].line.num = (symbol->memory_address) << 3;
                        code[(*C)].line.this_word.ARE = R;
                    }
                }
                second_word = TRUE;
            }
        }
        else
        {
            (*C)++;
            second_word = TRUE;
        }
    }
    if(data.destination[0] != '\0') {
        /* checks if destination is a number */
        if (data.destination[0] != '#') {
            /* checks if destination is a register */
            int tmp = CheckIfDestinationRegister(data);
            if (tmp == FALSE) {
                /* checking if we already added an extra word */
                if (second_word == TRUE) {
                    if (code[(*C)-1].line.this_word.method_des == direct) {
                        ptr symbol = getAddressOfSymbol(symbol_list, data.destination);
                        if (symbol == NULL) {
                            fprintf(stdout, "%d: Error: Symbol %s was not found on the symbol table.\n", line_index,
                                    data.destination);
                            (*C)++;
                            return FALSE;
                        }
                        if (symbol->isExtern) {
                            (*C)++;
                            /* memory address is at max 21 bits and words are 24 bits. */
                            code[(*C)].line.num = (symbol->memory_address) << 3;
                            code[(*C)].line.this_word.ARE = E;
                            strcpy(code[(*C)].name, symbol->name);

                        } else {
                            (*C)++;
                            /* memory address is at max 21 bits and words are 24 bits. */
                            code[(*C)].line.num = (symbol->memory_address) << 3;
                            code[(*C)].line.this_word.ARE = R;
                        }
                    }
                }
                else{
                    if (code[(*C)].line.this_word.method_des == direct) {
                        ptr symbol = getAddressOfSymbol(symbol_list, data.destination);
                        if (symbol == NULL) {
                            fprintf(stdout, "%d: Error: Symbol %s was not found on the symbol table.\n", line_index,data.destination);
                            (*C)++;
                            return FALSE;
                        }
                        if (symbol->isExtern) {
                            (*C)++;
                            /* memory address is at max 21 bits and words are 24 bits. */
                            code[(*C)].line.num = (symbol->memory_address) << 3;
                            code[(*C)].line.this_word.ARE = E;
                            strcpy(code[(*C)].name, symbol->name);
                        } else {
                            (*C)++;
                            /* memory address is at max 21 bits and words are 24 bits. */
                            code[(*C)].line.num = (symbol->memory_address) << 3;
                            code[(*C)].line.this_word.ARE = R;
                        }
                    }
                    if (code[(*C)].line.this_word.method_des == relative) {
                        char *name = data.destination + 1; /* getting the symbol name without the & sign */
                        ptr symbol = getAddressOfSymbol(symbol_list, name);
                        if (symbol == NULL) {
                            fprintf(stdout, "%d: Error: Symbol %s was not found on the symbol table.\n", line_index,
                                    data.destination);
                            (*C)++;
                            return FALSE;
                        }
                        if (symbol->isExtern) {
                            fprintf(stdout, "%d: Error: relative method can't get an external symbol.\n", line_index);
                            (*C)++;
                            return FALSE;
                        } else {
                            (*C)++;
                            code[(*C)].line.num = (((symbol->memory_address) - (IC_DEFAULT_VALUE + (*C) - 1)) << 3);
                            code[(*C)].line.this_word.ARE = A;
                        }
                    }
                }
            }
        } else
            (*C)++;
    }
    return no_error;

}

/* checks if the source is a register */
int CheckIfSourceRegister(DATA data)
{
    int i;
    for (i = 0;i < NUM_REGISTERS;i++)
    {
        char * registers[] = REGISTERS;
        if(strcmp(registers[i],data.source) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/* checks if the destination is a register */
int CheckIfDestinationRegister(DATA data)
{
    int i;
    for (i = 0;i < NUM_REGISTERS;i++)
    {
        char * registers[] = REGISTERS;
        if(strcmp(registers[i],data.destination) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}






