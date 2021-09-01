#ifndef ASSEMBLER_ITERATION1_H
#define ASSEMBLER_ITERATION1_H

/*******************************************************************************************
 *
 * Iteration1 is a manager for first scan of assembly files
 *
 *******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "GlobalParams.h"
#include "SymbolMngr.h"
#include "LineParser.h"
#include "DataMngr.h"
#include "WordMngr.h"

int iteration1(FILE *fp, ptr *symbol_list, MachineCode *code, int *dataImage, int *IC, int *DC);


#endif
