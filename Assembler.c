/***************************************************************************************
 *
 * Assembler transforms assembly files to machine code
 *
 ****************************************************************************************/


#include "Assembler.h"

int main(int argc, char **argv) {
    unsigned int i;
    printf("Starting Assembler!\n");
    for (i = 1; i < argc; i++) {
        assemble_file(argv[i]);
    }
    return 0;
}

/*
 * Function to run all assembling functions for a single file
 */
void assemble_file(char *file_path) {
    ptr symbol_list = NULL;
    int IC = IC_DEFAULT_VALUE;
    int DC = DC_DEFAULT_VALUE;
    int dataImage[MAX_LINES_IN_CODE] = {EOF};
    MachineCode code[MAX_LINES_IN_CODE] = {0};
    FILE *fp;
    line_index = 0;

    fp = RunFileMngr(file_path);
    if (fp == NULL) {
        return;
    }

    if (!iteration1(fp, &symbol_list, code, dataImage, &IC, &DC)) {
        printf("Error in 1st iteration, moving to next file\n");
        freeSymbolList(&symbol_list);
        closeFile(fp);
        return;
    }
    updateDataSymbols(&symbol_list, IC);
    if (!iteration2(fp, code, &symbol_list, &IC)) {
        printf("Error in 2nd iteration, moving to next file\n");
        freeSymbolList(&symbol_list);
        closeFile(fp);
        return;
    }
    outputFiles(file_path, symbol_list, code, dataImage, IC, DC);
    closeFile(fp);
    freeSymbolList(&symbol_list);


    printf("Done assembling file:\t%s.as\n", file_path);
}



