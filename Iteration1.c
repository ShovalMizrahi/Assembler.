/*******************************************************************************************
 *
 * Iteration1 is a manager for first scan of assembly files
 *
 *******************************************************************************************/

#include "Iteration1.h"

/*
 *  Function iterates on file, parses data and fills data structs
 */
int iteration1(FILE *fp, ptr *symbol_list, MachineCode *code, int *dataImage, int *IC, int *DC) {

    int res = 0;
    int noErrors = TRUE;
    int code_counter = 0;
    line_index = 0;


    while (res != EOF) {
        DATA data = {0};

        line_index++;
        res = readLine(fp, &data);
        if (res == skip_next_line_error) {
            noErrors = FALSE;
            continue;
        } else if (res == skip_next_line_no_error) {
            continue;
        }
        if (data.isEntry) {
            continue;
        }
        if (data.hasSymbol) {
            if (!RunSymbolMngr(symbol_list, data, *IC, *DC)) {
                noErrors = FALSE;
                continue;
            }
        }
        if (data.isString || data.isData) {
            if (!RunDataMngr(data, dataImage, DC)) {
                noErrors = FALSE;
                continue;
            }
        }
        if (data.isInstruction && !RunWordMngr(data, code, &code_counter)) {
            noErrors = FALSE;
            continue;
        }
        (*IC) += data.numWords;
        free(data.Data);

    }
    return noErrors;
}







