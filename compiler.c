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
extern void printSymbolTable();

int main()
{
    int lineNumber = 0;

    parseSingleLine(".entry      x", ++lineNumber);
    parseSingleLine("x:            .data          3        ", ++lineNumber);
    parseSingleLine("x:            .string          3        ", ++lineNumber);
    parseSingleLine("y:            .data          5, 8,     56        ", ++lineNumber);
    parseSingleLine("d:            .data          5, $,     56        ", ++lineNumber);
    parseSingleLine("z:            .data          7      ,  ,   56,        ", ++lineNumber);
    parseSingleLine("fg:            .data          7      ,  ,   44,$^        ", ++lineNumber);
    parseSingleLine("g:            .data          7      ,  ,   ,,,56,        ", ++lineNumber);
    parseSingleLine(".external Bla", ++lineNumber);
    parseSingleLine(".external stamm      ddd   ", ++lineNumber);
    parseSingleLine(".external dsafs   ", ++lineNumber);
    parseSingleLine("str:            .string          \"abcd\"        ", ++lineNumber);
    parseSingleLine("str2:            .string          \"AAADDDDDabcd\"        ", ++lineNumber);
    parseSingleLine("str3:            .string          \"FFFFFFFFAAADDDDDabcd\"        ", ++lineNumber);
    printSymbolTable();

    return 0;
}
