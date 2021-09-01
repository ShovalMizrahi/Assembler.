/**************************************************************************************************
 *
 * Container of helper functions for line parser
 *
 **************************************************************************************************/

#include "LineParser.h"


/*
 * Verify that instruction is Data type
 */
int isDataInstruction(const char *array) {
    char *isString = strstr(array, ".data");
    if (isString != NULL) {
        return TRUE;
    }
    return FALSE;
}

/*
 * Verify line is of type comment
 * All characters are white spaces until first non-white space character which is ';'
 */
int isComment(const char *array) {
    int i;
    for (i = 0; array[i]; i++) {
        if (array[i] == ';') return TRUE;
        if (!isspace(array[i])) return FALSE;
    }
    return FALSE;
}

/*
 * Verify if line is of type string if '.string' is in line
 */
int isStringInstruction(const char *array) {
    char *isString_ptr = strstr(array, ".string");
    if (isString_ptr != NULL) {
        return (int) (isString_ptr - array);
    }
    return -1;
}

/*
 * Functions finds if upto defined character there are no white-spaces
 * If there are white spaces before defined character: return False
 * If the defined character doesn't exist: return False
 * else return True
 */
int CharExistsNoWhiteSpaces(const char buffer[], char c) {
    int i;
    for (i = 0; buffer[i]; i++) {
        if (buffer[i] == c) {
            return TRUE;
        }
        if (isspace(buffer[i])) return FALSE;
    }
    return FALSE;
}

/*
 * Function verifies all characters upto index are white-spaces
 */
int isWhiteSpacesUpToIndex(const char *array, int index) {
    int i;
    for (i = 0; i < index && array[i]; i++) {
        if (!isspace(array[i])) return FALSE;
    }
    return TRUE;
}

/*
 * Check if first non-white space character is the desired character
 */
int charExistsOnlyWhiteSpaces(const char buffer[], char c) {
    int i;
    for (i = 0; buffer[i]; i++) {
        if (buffer[i] == c) {
            return TRUE;
        }
        if (!isspace(buffer[i])) return FALSE;
    }
    return FALSE;
}

/*
 * Check if last non-white space character is the desired character
 */
int charExistsOnlyWhiteSpacesR(const char buffer[], char c) {
    int i;
    for (i = (int) strlen(buffer) - 1; i >= 0; i--) {
        if (buffer[i] == c) {
            return TRUE;
        }
        if (!isspace(buffer[i])) return FALSE;
    }
    return FALSE;
}

/*
 * Find left most location of a specific char
 * if char doesn't exists, return -1
 */
int getCharLeftMostLoc(const char *string, char c) {
    int i;
    for (i = 0; string[i]; i++) {
        if (string[i] == c) {
            return i;
        }
    }
    return -1;
}

/*
 * Find right most location of a specific char
 * if char doesn't exists, return -1
 */
int getCharRightMostLoc(const char *string, char c) {
    int i;
    for (i = (int) strlen(string) - 1; i >= 0; i--) {
        if (string[i] == c) {
            return i;
        }
    }
    return -1;
}

/*
 * check if array is not an op-code or a register saved name
 */
int isFreeWord(const char *array) {
    if (checkIfOpCode(array) || checkIfRegister(array) || checkIfSavedWord(array)) return FALSE;
    return TRUE;
}

/*
 * Check if array is using a saved word
 */
int checkIfSavedWord(const char *array) {
    int i;
    char *saved_words[] = SAVED_WORDS;
    for (i = 0; i < NUM_SAVED_WORDS; i++) {
        if (strcmp(array, saved_words[i]) == 0) return TRUE;
    }
    return FALSE;
}

/*
 * check if array is not an op-code saved name
 */
int checkIfOpCode(const char *array) {
    int i;
    char *opcodes[] = OPCODES;
    for (i = 0; i < NUM_OPCODES; i++) {
        if (strcmp(array, opcodes[i]) == 0) return TRUE;
    }
    return FALSE;
}

/*
 * find if opcode exists in array, if it is return location in array
 * if its not, return -1
 */
int getOpCodeLocInArr(const char *array) {

    char *opcodes[] = OPCODES;
    int i;
    char *res;
    for (i = 0; i < NUM_OPCODES; i++) {
        if ((res = strstr(array, opcodes[i]))) {
            return (int) (res - array);
        }
    }

    return -1;
}

/*
 * Function splits a string according to its most left comma index
 * comma it self is removed from both prefix and suffix.
 * if no comma is found - prefix contains entire array and suffix is an empty string
 */
int splitArraysByLeftMostChar(const char array[], char prefix[], char suffix[], char C) {
    int i;
    int ind = getCharLeftMostLoc(array, C);
    char buff[MAX_LINE_LENGTH] = {0};
    strcpy(buff, array);
    if (ind == -1) {
        suffix[0] = 0;
        strcpy(prefix, buff);
        return 0; /* No char in array */
    }
    for (i = 0; array[i]; i++) {
        suffix[i] = array[ind + i + 1];
    }
    strcpy(prefix, buff);
    prefix[ind] = 0;

    return 1;
}

char *cleanWhiteSpacesPrefixSuffix(char *array) {
    int i;
    for (i = (int) strlen(array) - 1; i >= 0; i--) {
        if (!isspace(array[i])) {
            array[i + 1] = 0;
            break;
        }
    }
    for (i = 0; array[i]; i++) {
        if (!isspace(array[i])) break;
    }
    return &array[i];
}

/*
 * Verify that array is only an integer
 */
int isOnlyInteger(const char *array) {
    int i;
    if (strlen(array) == 1) return isdigit(array[0]);
    for (i = 0; array[i]; i++) {
        if (!isspace(array[i])) {
            break;
        }
    }
    if (array[i] == '-' || array[i] == '+') {
        i++;
    }
    for (; array[i]; i++) {
        if (!isdigit(array[i])) {
            break;
        }
    }
    for (; array[i]; i++) {
        if (!isspace(array[i])) {
            return FALSE;
        }
    }

    return TRUE;
}

/*
 * Function verifies that string is a valid string
 */
int isValidString(const char *array, int left, int right) {
    char c = '\"';
    int loc = isStringInstruction(array) + (int) strlen(".string");
    if (!charExistsOnlyWhiteSpaces(&array[loc], c)) {
        printf("%d: Error: Non-white space characters between '.string' and first parentheses!\n", line_index);
        return FALSE;
    }
    if (left == -1) {
        printf("%d: Error: Missing parentheses for string!\n", line_index);
        return FALSE;
    }
    if (right == left) {
        printf("%d: Error: Missing closing parentheses for string!\n", line_index);
        return FALSE;
    }
    if (!charExistsOnlyWhiteSpacesR(&array[right], c)) {
        printf("%d: Error: Non-white space characters after last parentheses!\n", line_index);
        return FALSE;
    }
    return TRUE;
}

/*
 * function iterates on array and returns 1 if array is all-white space characters,
 * returns 0 if it has a single non-white character.
 */
int allWhiteSpaceInArray(const char *array) {
    int i;
    for (i = 0; array[i]; ++i) {
        if (!isspace(array[i])) return 0;
    }
    return 1;
}

/*
 * function verifies string is a valid label
 */
int isValidLabel(const char array[]) {
    int i;
    if (!isalpha(array[0])) {
        fprintf(stdout,
                "%d: Error: First character in symbol has to be an alphabetic character, first letter is '%c'\n",
                line_index, array[0]);
        return FALSE;
    }
    for (i = 1; array[i]; i++) {
        if (!isalnum(array[i])) {
            fprintf(stdout,
                    "%d: Error: Each character in symbol has to be an alphanumeric character, letter number %d is '%c'\n",
                    line_index, i + 1, array[i]);
            return FALSE;
        }
    }
    if (strlen(array) > MAX_LBL_SIZE - 1) {
        fprintf(stdout, "%d: Error: Maximum length for symbol is %d characters, received %lu\n", line_index,
                MAX_LBL_SIZE - 1, strlen(array));
        return FALSE;
    }

    if (!isFreeWord(array)) {
        fprintf(stdout, "%d: Error: Label '%s' is a saved word in assembly language\n", line_index, array);
        return FALSE;
    }

    return TRUE;
}

/*
 * Function verifies array is a predefined name of a register
 */
int checkIfRegister(const char *array) {
    int i;
    char *registers[] = REGISTERS;
    for (i = 0; i < NUM_REGISTERS; i++) {
        if (strcmp(array, registers[i]) == 0) return TRUE;
    }
    return FALSE;
}

/*
 * Add a valid number to dynamic array
 */
long *addNumbertoArray(int num, int *size, long *data) {
    long *ptr = NULL;
    if (data == NULL) {
        ptr = malloc(sizeof(long));
        if (ptr == NULL) {
            perror("Error - OUT OF MEMORY!");
            exit(0);
        }
        *ptr = num;
        *size = 1;
    } else {
        ptr = realloc(data, ((*size) + 1) * sizeof(long));
        if (!ptr) {
            perror("Error - OUT OF MEMORY!");
            exit(0);
        }
        *(ptr + *size) = num;
        (*size)++;
    }

    return ptr;
}

/*
 * Verify all characters in array are alpha-numeric
 */
int checkAllAlnum(const char *string) {
    int i;
    for (i = 0; string[i]; i++) {
        if (!isalnum(string[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

int isValidInteger(int num) {
    int max_num = (int) pow(2, NUM_BITS_IN_WORD - 1) - 1;
    int min_num = (int) -(pow(2, NUM_BITS_IN_WORD - 1));
    if (num > max_num || num < min_num) {
        return FALSE;
    }
    return TRUE;
}

/*
 * Verify all chars in array are non-white characters
 */
int allNonWhite(const char *array) {
    int i;
    for (i = 0; array[i]; i++) {
        if (isspace(array[i])) return FALSE;

    }
    return TRUE;
}

/*
 *  Function verifies if two characters 'c' are adjacent (separated only by whitespaces)
 */
int findTwoAdjacentChars(const char *array, char c) {
    int i = 0;
    /* break at first appearance of char */
    for (; array[i]; i++) {
        if (array[i] == c) {
            i++;
            break;
        }
    }
    /* if second appearance of char is before any other non-white space char - return FALSE, else return TRUE */
    for (; array[i]; i++) {
        if (array[i] == c) {
            return FALSE;
        }
        if (!isspace(array[i])) {
            break;
        }
    }
    return TRUE;
}

/*
 *  Check is extern is part of array, making it an extern cmd
 */
int isExternalCmd(const char *array) {
    char *isString_ptr = strstr(array, ".extern");
    if (isString_ptr != NULL) {
        return (int) (isString_ptr - array);
    }
    return -1;
}

/*
 * Calculate how many words of memory line will take (1-3 possible options)
 */
void calcNumWords(int *numWords, char *source, char *destination) {
    char regiesters[][SHORT_ARR_SIZE] = REGISTERS;
    int i;
    int sourceWord = 1;
    int destWord = 1;
    for (i = 0; regiesters[i][0]; i++) {
        if (strcmp(regiesters[i], source) == 0 || !strlen(source)) {
            sourceWord = 0;
        }
        if (strcmp(regiesters[i], destination) == 0 || !strlen(destination)) {
            destWord = 0;
        }
    }
    *numWords = 1 + sourceWord + destWord;

}

/*
 * Check if entry is part of array, making it a entry CMD
 */
int isEntryCmd(const char *array) {
    char *isString_ptr = strstr(array, ".entry");
    if (isString_ptr != NULL) {
        return (int) (isString_ptr - array);
    }
    return -1;
}

