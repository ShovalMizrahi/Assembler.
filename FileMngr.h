#ifndef ASSEMBLER_FILEMNGR_H
#define ASSEMBLER_FILEMNGR_H
/********************************************************************************
 *
 * File manager is a container of helper function for reading and writing files
 *
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GlobalParams.h"
#include "SymbolMngr.h"

FILE *RunFileMngr(char *path);

int outputFiles(char *path, ptr symbol_list, MachineCode *code, int *dataImage, unsigned int IC, unsigned int DC);

int printCodeExtToFile(FILE *code_file, FILE *ext_file, MachineCode *code, const int *data_image, unsigned int IC,
                       unsigned int DC);

int printEntToFile(FILE *fp, ptr *symbol_list);

void closeFile(FILE *fp);

#endif
