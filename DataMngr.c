#include "DataMngr.h"

/*************************************************************************
 *
 * DataMngr is responsible for adding all the data to the data_image
 * being called to only in the first scan.
 *
 *************************************************************************/

/* calls to the addData function. returns true */
int RunDataMngr(DATA data, int *data_image, int* dc)
{
    addData(data,data_image, dc);
    return TRUE;
}

/* adds the string/number to the data image */
void addData(DATA data,int * storage,int * dc)
{
    int i;
    if(data.isData)
    {
        for(i=0;i<data.size_data;i++)
        {
            storage[*dc] = *(data.Data+i);
            (*dc)++;
        }
        storage[*dc] = EOF;
    }
    if(data.isString)
    {
        for(i=0;i<data.string[i];i++)
        {
            storage[*dc] = (int)(data.string[i]);
            (*dc)++;
        }
        storage[*dc] = 0;
        (*dc)++;
        storage[*dc] = EOF;
    }
}


