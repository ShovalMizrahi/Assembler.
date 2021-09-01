
/*************************************************************************
 *
 * DataMngr is responsible for adding all the data to the data_image
 * being called to only in the first scan.
 *
 **************************************************************************/




#ifndef ASSEMBLER_DATAMNGR_H
#define ASSEMBLER_DATAMNGR_H
#include "GlobalParams.h"
#include <stdio.h>
#include <stdlib.h>

/*calls to the addData function. returns true */
int RunDataMngr(DATA data, int *data_image, int* dc);
/* adds the string/number to the data image */
void addData(DATA,int *,int *);


#endif
