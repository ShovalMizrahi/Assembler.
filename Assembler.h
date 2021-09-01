
#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H
/***************************************************************************************
 *
 * Assembler transforms assembly files to machine code
 *
 ****************************************************************************************/
#include "GlobalParams.h"
#include <stdio.h>
#include "Iteration1.h"
#include "Iteration2.h"
#include "SymbolMngr.h"
#include "FileMngr.h"


void assemble_file(char *file_path);

#endif

