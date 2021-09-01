/********************************************************************************
 *
 * File manager is a container of helper function for reading and writing files
 *
 *******************************************************************************/
#include "FileMngr.h"

/*
 * Read file from given input path, raise error if no file in path
 */
FILE *RunFileMngr(char *path) {
    FILE *fp;
    char *file_path = malloc(strlen(path) + strlen(".as") + 1);
    if (!file_path) {
        perror("Not enough memory!");
        exit(0);
    }
    sprintf(file_path, "%s.as", path);
    fp = fopen(file_path, "r");
    if (fp) printf("\nStarting to assemble file:\t%s\n", file_path);
    else fprintf(stdout, "Error - No file found in path %s!\n", file_path);
    free(file_path);
    return fp;
}

/*
 * Closes file
 */
void closeFile(FILE *fp) {
    fclose(fp);
}


/*
 * Function manages all output files, and writes data into them
 */
int outputFiles(char *path, ptr symbol_list, MachineCode *code, int *dataImage, unsigned int IC, unsigned int DC) {
    FILE *fp_out_code = NULL, *fp_out_extern = NULL, *fp_out_entry = NULL;
    char *file_path = malloc(strlen(path) + strlen(".ent") + 1);
    if (!file_path) {
        perror("Not enough memory!");
        exit(0);
    }
    sprintf(file_path, "%s.ob", path);
    fp_out_code = fopen(file_path, "w");
    if (hasExtern(&symbol_list)) {
        sprintf(file_path, "%s.ext", path);
        fp_out_extern = fopen(file_path, "w");
    }
    printCodeExtToFile(fp_out_code, fp_out_extern, code, dataImage, IC, DC);

    if (hasEntry(&symbol_list)) {
        sprintf(file_path, "%s.ent", path);
        fp_out_entry = fopen(file_path, "w");
        printEntToFile(fp_out_entry, &symbol_list);
        fclose(fp_out_entry);
    }


    free(file_path);
    fclose(fp_out_code);
    if (fp_out_extern)
        fclose(fp_out_extern);

    return FALSE;
}

/*
 * Function prints Code to ob file and external symbols to EXT file
 */
int printCodeExtToFile(FILE *code_file, FILE *ext_file, MachineCode *code, const int *data_image, unsigned int IC,
                       unsigned int DC) {
    int i;
    fprintf(code_file, "%7d %d\n", IC - IC_DEFAULT_VALUE, DC);
    /* print code */
    for (i = 0; i < MAX_LINES_IN_CODE; i++) {
        /* If num in line is 0 - no more lines */
        if (code[i].line.num == 0)
            break;
        /* convert i and num in line to line of machine code */
        fprintf(code_file, "%07d %06lx\n", i + 100, code[i].line.num & 0xffffff);
        /* if field name is not 0, meaning there's an external cmd here, print it to file */
        if (ext_file && code[i].name[0] != 0) {
            fprintf(ext_file, "%s %07d\n", code[i].name, i + 100);
        }
    }
    /* print data after code */
    for (i = 0; i < MAX_LINES_IN_CODE; i++) {
        if (data_image[i] == EOF)
            break;
        fprintf(code_file, "%07d %06x\n", i + IC, data_image[i] & 0xffffff);

    }
    return TRUE;
}


/*
 *  Function prints Entry symbols to file
 */
int printEntToFile(FILE *fp, ptr *symbol_list) {
    ptr *iter = symbol_list;
    while (*iter != NULL) {
        if ((*iter)->isEntry) {
            fprintf(fp, "%s %07d\n", (*iter)->name, (*iter)->memory_address);
        }
        iter = &(*iter)->next;
    }

    return TRUE;
}



