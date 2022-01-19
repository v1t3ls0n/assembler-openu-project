#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
    unsigned int Address;
    unsigned int A : 4;
    unsigned int B : 4;
    unsigned int C : 4;
    unsigned int D : 4;
    unsigned int E : 4;
} SingleWordObjFormat;

void printObjectFile(SingleWordObjFormat obj[], unsigned int ICF, unsigned int DCF);