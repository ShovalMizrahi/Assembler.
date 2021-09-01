/*****************************************************************************************************
 *
 * Symbol manager manages the symbol list of assembler, using a linked-list
 * During first iteration1:
 * Add all non-entry symbols to list
 * During second-iteration2:
 * Add all entry symbols to existing symbols in list
 *
 *****************************************************************************************************/
#ifndef ASSEMBLER_SYMBOLMNGR_H
#define ASSEMBLER_SYMBOLMNGR_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GlobalParams.h"

int RunSymbolMngr(ptr *symbol_list, DATA data, unsigned int IC, unsigned int DC);

int checkIfSymbolIsDuplicate(ptr *symbol_list, DATA data);

void addToList(ptr *pSymbolList, ptr *new_symbol);

void freeSymbolList(ptr *pSymbolList);

int updateSymbolWithEntry(ptr *head, char *symbol);

int hasEntry(ptr *symbol_list);

int hasExtern(ptr *symbol_list);

int updateDataSymbols(ptr *symbol_list, unsigned int IC);

void fillSymbol(DATA *data, unsigned int IC, unsigned int DC, ptr new_symbol);

ptr getAddressOfSymbol(ptr *symbol_list, char *symbol_name);

#endif
