/*****************************************************************************************************
 *
 * Symbol manager manages the symbol list of assembler, using a linked-list
 * During first iteration1:
 * Add all non-entry symbols to list
 * During second-iteration2:
 * Add all entry symbols to existing symbols in list
 *
 *****************************************************************************************************/

#include "SymbolMngr.h"


/*
 * Symbol manager operating function given a new data input
 * 1. allocate memory for new symbol
 * 2. if data is entry - find symbol in linked-list and update that it is entry (if missing - raise error)
 * 3. look for duplicates - if duplicates are found (which are not extern/entry) - raise error
 * 4. add new symbol to linked-list as tail *
 */
int RunSymbolMngr(ptr *symbol_list, DATA data, unsigned int IC, unsigned int DC) {
    ptr new_symbol = malloc(sizeof(item));
    if (!new_symbol) {
        /* severe memory error - meaning no capability to operate, exit program */
        perror("Error - OUT OF MEMORY!");
        exit(1);
    }

    if (data.isEntry) {

        if (!updateSymbolWithEntry(symbol_list, data.Symbol)) {
            /* If here - the symbol is entry, but not in symbol list, this is an error */
            printf("%d: Error: Entry symbol '%s' not declared\n", line_index, data.Symbol);
            free(new_symbol);
            return FALSE;
        }
        return TRUE;
    }
    if (checkIfSymbolIsDuplicate(symbol_list, data)) {
        /* Symbol is a duplicate*/
        printf("%d: Error: Multiple uses of symbol '%s'\n", line_index, data.Symbol);
        free(new_symbol);
        return FALSE;
    }

    /* Symbol is not a duplicate and can be added to linked list, prepare data and add to linked list*/
    fillSymbol(&data, IC, DC, new_symbol);
    addToList(symbol_list, &new_symbol);
    return TRUE;
}

/*
 * convert data from "DATA" struct to "symbol_list" struct which contains only required information for symbol
 */
void fillSymbol(DATA *data, unsigned int IC, unsigned int DC, ptr new_symbol) {
    strcpy(new_symbol->name, (*data).Symbol);
    if ((*data).isExtern) {
        /* external symbol's memory address is always 0 */
        new_symbol->memory_address = 0;
    } else if ((*data).isString || (*data).isData) {
        /* String/Data symbol's memory address is set by DC */
        new_symbol->memory_address = DC;
        new_symbol->isCode = FALSE;
    } else {
        /* instruction symbol's memory address is set by IC */
        new_symbol->memory_address = IC;
        new_symbol->isCode = TRUE;
    }
    new_symbol->isExtern = (*data).isExtern;
    new_symbol->isEntry = (*data).isEntry;
    new_symbol->next = NULL;
}

/*
 * Free memory of symbol list object's on the heap
 */
void freeSymbolList(ptr *pSymbolList) {
    ptr iter = *pSymbolList;
    ptr tmp;
    while ((iter) != NULL) {
        tmp = (*iter).next;
        iter->next = NULL;
        free(iter);
        iter = tmp;
    }
    if (*pSymbolList != NULL) *pSymbolList = NULL;
}


/*
 * Add a new item to list
 */
void addToList(ptr *pSymbolList, ptr *new_symbol) {
    ptr *iter = pSymbolList;
    if ((*iter) == NULL) {
        /* List is empty, set item to head and return */
        *pSymbolList = *new_symbol;
        return;
    }
    while ((*iter)->next != NULL) {
        /* List is not empty, iterate list until tail is reached */
        iter = &(*iter)->next;
    }
    if ((*iter)->next == NULL) {
        /* set current tail's 'next' to new_symbol - making it the new tail */
        (*iter)->next = *new_symbol;
    }
}

/*
 * Iterate linked-list and return TRUE if an item in list has the same name as given symbol_name
 */
int checkIfSymbolIsDuplicate(ptr *symbol_list, DATA data) {
    ptr *iter = symbol_list;
    while (*iter != NULL) {
        if (strcmp((*iter)->name, data.Symbol) == 0) {
            if ((*iter)->isExtern && data.isExtern);
            else if ((*iter)->isEntry && data.isEntry);
            else return TRUE;
        }
        iter = &(*iter)->next;

    }

    return FALSE;

}

/*
 * Iterate linked-list and find item with same name as symbol
 * if its found - make it an entry symbol and return TRUE
 * if no item is found true - return FALSE
 */
int updateSymbolWithEntry(ptr *head, char *symbol) {
    ptr *iter = head;
    while (*iter != NULL) {
        if (strcmp((*iter)->name, symbol) == 0) {
            (*iter)->isEntry = TRUE;
            return TRUE;
        }
        iter = &(*iter)->next;
    }
    return FALSE;
}

/*
 * Verify at least one item in linked-list is an Entry symbol
 */
int hasEntry(ptr *symbol_list) {
    ptr *iter = symbol_list;
    while (*iter != NULL) {
        if ((*iter)->isEntry) {
            return TRUE;
        }
        iter = &(*iter)->next;
    }
    return FALSE;
}

/*
 * Verify at least one item in linked-list is an External symbol
 */
int hasExtern(ptr *symbol_list) {
    ptr *iter = symbol_list;
    while (*iter != NULL) {
        if ((*iter)->isExtern) {
            return TRUE;
        }
        iter = &(*iter)->next;
    }
    return FALSE;
}

/*
 * Iterate linked-list and if symbol is data (DATA/STRING) than add the final IC value to its memory address
 */
int updateDataSymbols(ptr *symbol_list, unsigned int IC) {
    ptr *iter = symbol_list;
    while (*iter != NULL) {
        if (!(*iter)->isCode && !(*iter)->isExtern) {
            (*iter)->memory_address += IC;
        }
        iter = &(*iter)->next;
    }
    return TRUE;
}

/*
 * Return memory address of symbol by name
 * if symbol's name is not in list - return -1
 */
ptr getAddressOfSymbol(ptr *symbol_list, char *symbol_name) {
    ptr *iter = symbol_list;
    while (*iter != NULL) {
        if (strcmp((*iter)->name, symbol_name) == 0) {
            return *iter;
        }
        iter = &(*iter)->next;

    }

    return NULL;

}
