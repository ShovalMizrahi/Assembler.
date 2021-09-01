#ifndef ASSEMBLER_LINEPARSER_H
#define ASSEMBLER_LINEPARSER_H

/**********************************************************************************************************************
 *
 * Line parser reads a new line and parses line to data type, label, instruction/data commands, source and destination
 *
 *********************************************************************************************************************/

#include "GlobalParams.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>


int readLine(FILE *fp, void *data);
int parseLine(char *line, DATA *data_ptr);
int parseExternEntry(char buffer[], unsigned int *hasSymbol, char symbol[], char *externEntry);
int allWhiteSpaceInArray(const char *array);
int parseSymbol(char buffer[], unsigned int *hasSymbol, char symbol[]);
int getCharLeftMostLoc(const char *string, char c);
int getCharRightMostLoc(const char *string, char c);
int isValidLabel(const char array[]);
int isFreeWord(const char *array);
int parseOpCode(char array[], char *opcode, unsigned int isSymbol);
int checkIfOpCode(const char *array);
int checkIfRegister(const char *array);
int parseDestinationSource(char *array, char *destination, char *source);
int parseString(const char *array, char *string);
int isComment(const char *array);
int isStringInstruction(const char *array);
int isDataInstruction(const char *array);
int CharExistsNoWhiteSpaces(const char buffer[], char c);
int charExistsOnlyWhiteSpaces(const char buffer[], char c);
int charExistsOnlyWhiteSpacesR(const char buffer[], char c);
int parseDataInstruction(char *array, int *size, long **data);
int isOnlyInteger(const char *array);
long *addNumbertoArray(int num, int *size, long *data);
int splitArraysByLeftMostChar(const char array[], char prefix[], char suffix[], char C);
int getOpCodeLocInArr(const char *array);
char *cleanWhiteSpacesPrefixSuffix(char *array);
int isValidString(const char *array, int left, int right);
int isExternalCmd(const char *string);
int checkAllAlnum(const char *string);
void calcNumWords(int *numWords, char *source, char *destination);
int isEntryCmd(const char *array);
int isWhiteSpacesUpToIndex(const char *array, int index);
int isValidInteger(int num);
int checkIfSavedWord(const char *);
int allNonWhite(const char *array);
int findTwoAdjacentChars(const char *array, char c);

#endif
