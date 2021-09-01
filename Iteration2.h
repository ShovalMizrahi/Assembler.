
#ifndef ASSEMBLER_ITERATION2_H
#define ASSEMBLER_ITERATION2_H

#include <stdio.h>
#include <stdlib.h>
/*******************************************************************************************
 *
 * Iteration2 is a manager for second scan of assembly files
 *
 *******************************************************************************************/
#include "GlobalParams.h"
#include "LineParser.h"
#include "SymbolMngr.h"
#include "WordMngr.h"

int iteration2(FILE *fp, MachineCode *code, ptr *symbol_list, int *IC);


#endif
