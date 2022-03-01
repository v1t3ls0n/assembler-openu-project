#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern Error currentError;
extern Item *symbols[HASHSIZE];
extern Item *macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];
extern unsigned char dec2Bin2sComplement(int n);
extern void printMemoryStacks(EncodingFormat format);

int main()
{
    int lineNumber = 1;
    parseSingleLine("x:            .data          3        ", ++lineNumber);
    parseSingleLine("y:            .data          5, 8,     56        ", ++lineNumber);
    parseSingleLine("z:            .data          7      ,  ,   56,        ", ++lineNumber);
    parseSingleLine("str:            .string          \"abcd\"        ", ++lineNumber);

    return 0;
}
