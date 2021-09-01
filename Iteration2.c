#include "Iteration2.h"

/*******************************************************************************************
 *
 * Iteration2 is a manager for second scan of assembly files
 *
 *******************************************************************************************/

int iteration2(FILE *fp, MachineCode *code, ptr *symbol_list, int *IC) {
    int res = 0;
    int noErrors = TRUE;
    int code_counter = -1;

    line_index = 0;

    rewind(fp);

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
        if (data.isData || data.isExtern || data.isString) {
            continue;

        }
        if (data.hasSymbol && data.isEntry) {
            if (!RunSymbolMngr(symbol_list, data, 0, 0)) {
                noErrors = FALSE;
                continue;
            }
        }
        if (data.isInstruction && !RunWordMngr_iter2(code, symbol_list, data, &code_counter)) {
            noErrors = FALSE;
            continue;
        }
    }
    return noErrors;
}







