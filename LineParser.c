/**********************************************************************************************************************
 *
 * Line parser reads a new line and parses line to data type, label, instruction/data commands, source and destination
 *
 *********************************************************************************************************************/

#include <memory.h>
#include "LineParser.h"

/*
 * Read a new line, remove suffix '\n' if its there, and parse it
 */
int readLine(FILE *fp, void *data_ptr) {
    char line[ARR_SIZE];
    char *res = fgets(line, ARR_SIZE, fp);

    if (res == NULL) {
        return EOF;  /* end of file */
    }
    /* remove \n at end of line */
    if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = 0;
    }
    /* Fix for comparability issues regarding next line */
    if (line[strlen(line) - 1] == '\r') {
        line[strlen(line) - 1] = 0;
    }
    return parseLine(line, data_ptr);
}

/*
 * Parse line according to line's syntax and semantics, illegal lines prints and error and returns
 */
int parseLine(char *line, DATA *data_ptr) {
    if (allWhiteSpaceInArray(line)) {
        /* empty line, skip without and error */
        return skip_next_line_no_error;
    }
    if (isComment(line)) {
        /* comment line (starts with ';') */
        return skip_next_line_no_error;
    }
    if (strlen(line) > MAX_LINE_LENGTH) {
        /* Line over allowed number of characters (80) for each line*/
        fprintf(stdout, "%d: Error: Line is too long! limited to %d characters in each line!\n", line_index,
                MAX_LINE_LENGTH);
        return skip_next_line_error;
    }
    if (isStringInstruction(line) != -1) {
        /* is string since ".string" is in line*/
        data_ptr->isString = TRUE;
        if (!parseSymbol(line, &data_ptr->hasSymbol, data_ptr->Symbol)) return skip_next_line_error;
        if (!parseString(line, data_ptr->string)) return skip_next_line_error;
    } else if (isExternalCmd(line) != -1) {
        /* Is external command, since ".extern" is in line*/
        data_ptr->isExtern = TRUE;
        /* Check for symbol as prefix, this is only for warning printing */
        if (!parseSymbol(line, &data_ptr->hasSymbol, data_ptr->Symbol)) return skip_next_line_error;
        if (data_ptr->hasSymbol)
            printf("%d: Warning: Label '%s' to external command is ignored\n", line_index, data_ptr->Symbol);
        if (!parseExternEntry(line, &data_ptr->hasSymbol, data_ptr->Symbol, ".extern")) return skip_next_line_error;
    } else if (isEntryCmd(line) != -1) {
        /* is entry command, since ".entry" is in line */
        data_ptr->isEntry = TRUE;
        /* Check for symbol as prefix, this is only for warning printing */
        if (!parseSymbol(line, &data_ptr->hasSymbol, data_ptr->Symbol)) return skip_next_line_error;
        if (data_ptr->hasSymbol)
            printf("%d: Warning: Label '%s' to entry command is ignored\n", line_index, data_ptr->Symbol);
        if (!parseExternEntry(line, &data_ptr->hasSymbol, data_ptr->Symbol, ".entry")) return skip_next_line_error;
    } else if (isDataInstruction(line)) {
        /* is data command, since ".data" in in line */
        data_ptr->isData = TRUE;
        if (!parseSymbol(line, &data_ptr->hasSymbol, data_ptr->Symbol)) return skip_next_line_error;
        if (!parseDataInstruction(line, &data_ptr->size_data, &data_ptr->Data)) return skip_next_line_error;
    } else {
        /* instruction command */
        data_ptr->isInstruction = TRUE;
        if (!parseSymbol(line, &data_ptr->hasSymbol, data_ptr->Symbol)) return skip_next_line_error;
        if (!parseOpCode(line, data_ptr->opCode, data_ptr->hasSymbol)) return skip_next_line_error;
        if (strlen(line) != 0 && !parseDestinationSource(line, data_ptr->destination, data_ptr->source))
            return skip_next_line_error;
        calcNumWords(&data_ptr->numWords, data_ptr->source, data_ptr->destination);
    }
    /* if here meaning parsing is successful and also need to continue with first iteration */
    return do_next_line;
}

/* parse the phrase ".entry" or ".extern", and find the symbol afterwards */
int parseExternEntry(char buffer[], unsigned int *hasSymbol, char symbol[], char *externEntry) {
    int loc = (isEntryCmd(buffer) > isExternalCmd(buffer) ? isEntryCmd(buffer) : isExternalCmd(buffer)) +
              (int) strlen(externEntry);
    char *symbol_ptr;
    symbol_ptr = cleanWhiteSpacesPrefixSuffix(&buffer[loc]);
    if (!isalpha(symbol_ptr[0])) {
        printf("%d: Error: First character in label has to be alphabetic!\n", line_index);
        return FALSE;
    }
    if (!checkAllAlnum(symbol_ptr)) {
        printf("%d: Error: All characters in label must be alpha-numeric!\n", line_index);
        return FALSE;
    }
    *hasSymbol = TRUE;
    strcpy(symbol, symbol_ptr);
    return TRUE;

}

/* checks and parses symbol */
int parseSymbol(char buffer[], unsigned int *hasSymbol, char symbol[]) {
    const char *s = ":";
    int leftCol;
    char *token;
    int leftParantheses = getCharLeftMostLoc(buffer, '\"');
    *hasSymbol = TRUE;

    if ((leftCol = getCharLeftMostLoc(buffer, s[0])) == -1) {
        /* No ':' in line, no symbol */
        *hasSymbol = FALSE;
        return TRUE;
    }

    if (leftCol > leftParantheses && leftParantheses != -1) {
        /* ':' is in string */
        *hasSymbol = FALSE;
        return TRUE;
    }


    if (!CharExistsNoWhiteSpaces(buffer, s[0])) {
        printf("%d: Error: Symbol name can't contain white-spaces \n", line_index);
        return FALSE;
    }

    /* get the first token */
    token = strtok(buffer, s);
    if (isValidLabel(token)) {
        strcpy(symbol, token);
    } else {
        *hasSymbol = FALSE;
        return FALSE;
    }
    token = strtok(NULL, s);
    strcpy(buffer, token);
    token = strtok(NULL, s);

    /* if any more tokens exist, so an additional ':' is in char array, output error */
    if (token != NULL) {
        fprintf(stdout, "%d: Error: Illegal syntax, too many ':', limited to exactly 1 ':' right after Label\n",
                line_index);
        *hasSymbol = FALSE;
        return FALSE;
    }
    return TRUE;
}

/*
 *  parse opcode, if valid put it in opcode variable, and the rest put in array
 */
int parseOpCode(char array[], char *opcode, unsigned int isSymbol) {
    char c = ' ';
    int loc;
    if ((loc = getOpCodeLocInArr(array)) != -1) {
        /* if there's no symbol, check if all characters are whitespaces upto location*/
        if (!isWhiteSpacesUpToIndex(array, loc)) {
            fprintf(stdout, "%d: Error: Excluding a symbol, no non-white characters are allowed before opcode\n",
                    line_index);
            return FALSE;
        }
        splitArraysByLeftMostChar(&array[loc], opcode, array, c);
        if (checkIfOpCode(opcode)) return TRUE;
    }
    fprintf(stdout, "%d: Error: undefined directive\n", line_index);
    return FALSE;
}

/*
 * Parse the destination and source to variables
 */
int parseDestinationSource(char *array, char *destination, char *source) {
    char *s = ",";
    char *token;
    array = cleanWhiteSpacesPrefixSuffix(array);
    if (charExistsOnlyWhiteSpaces(array, s[0]) || charExistsOnlyWhiteSpacesR(array, s[0])) {
        printf("%d: Error: first and last character after opcode cannot be ','\n", line_index);
        return FALSE;
    }
    if (getCharLeftMostLoc(array, s[0]) != -1) {
        if (!findTwoAdjacentChars(array, ',')) {
            printf("%d: Error: two consecutive commas are not allowed\n", line_index);
            return FALSE;
        }
        token = strtok(array, s);
        if (token) {
            if (allWhiteSpaceInArray(token)) {
                printf("%d: Error: 1st operand missing\n", line_index);
                return FALSE;
            }
            token = cleanWhiteSpacesPrefixSuffix(token);
            strcpy(source, token);
        }
        token = strtok(NULL, s);
        if (token) {
            if (allWhiteSpaceInArray(token)) {
                printf("%d: Error: 2nd operand missing\n", line_index);
                return FALSE;
            }
            token = cleanWhiteSpacesPrefixSuffix(token);
            strcpy(destination, token);
        }
        token = strtok(NULL, s);
        if (token) {
            printf("%d: Error: Only 2 arguments allowed\n", line_index);
            return FALSE;
        }


    } else {
        array = cleanWhiteSpacesPrefixSuffix(array);
        strcpy(destination, array);
        if ((!allNonWhite(destination))) {
            printf("%d: Error: operand can only be 1 word\n", line_index);
            return FALSE;
        }
        source[0] = 0;
    }
    return TRUE;
}

/*
 * Parse data instruction
 */
int parseDataInstruction(char *array, int *size, long **data) {
    char *token;
    char s = ',';
    char *ind = strstr(array, ".data") + strlen(".data");
    int i;
    for (i = 0; i < strstr(array, ".data") - array; i++) {
        if (!isspace(array[i])) {
            printf("%d: Error: excluding symbol, no leading characters which are non-white before .data are allowed\n",
                   line_index);
            return FALSE;
        }
    }
    /* Verify that both from right and left sides there's no commas */
    if (charExistsOnlyWhiteSpaces(ind, s) || charExistsOnlyWhiteSpacesR(ind, s)) {
        printf("%d: Error: first and last character after '.data' cannot be ','\n", line_index);
        return FALSE;
    }
    token = strtok(ind, &s);
    while (token != NULL) {
        if (isOnlyInteger(token)) {
            if (!isValidInteger(atoi(token))) {
                printf("%d: Error: Data overflow\n", line_index);
                return FALSE;
            }
            *data = addNumbertoArray(atol(token), size, *data);
        } else {
            printf("%d: Error: Invalid characters for data instruction\n", line_index);
            return FALSE;
        }
        token = strtok(NULL, &s);
    }
    return TRUE;
}

/*
 * Parse string instruction
 */
int parseString(const char *array, char *string) {
    char c = '\"';
    int right;
    int left;
    int loc = isStringInstruction(array) + (int) strlen(".string");
    left = loc + getCharLeftMostLoc(&array[loc], c) + 1;
    right = loc + getCharRightMostLoc(&array[loc], c);
    if (isValidString(array, left, right)) {
        strncpy(string, array + left, (size_t) (right - left));
        return TRUE;
    }
    return FALSE;
}

