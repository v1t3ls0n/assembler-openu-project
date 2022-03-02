#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern Item *symbols[HASHSIZE];
extern Item *macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];
extern unsigned char dec2Bin2sComplement(int n);
extern void printMemoryStacks(EncodingFormat format);
extern void printSymbolTable();

int main()
{

    parseSingleLine(".entry      x");
    parseSingleLine("x:            .data          3        ");
    parseSingleLine("x:            .string          3        ");
    parseSingleLine("y:            .data          5, 8,     56        ");
    parseSingleLine("d:            .data          5, $,     56        ");
    parseSingleLine("z:            .data          7      ,  ,   56,        ");
    parseSingleLine("fg:            .data          7      ,  ,   44,$^        ");
    parseSingleLine("g:            .data          7      ,  ,   ,,,56,        ");
    parseSingleLine(".external Bla");
    parseSingleLine(".external stamm      ddd   ");
    parseSingleLine(".external dsafs   ");
    parseSingleLine("str:            .string          \"abcd\"        ");
    parseSingleLine("str2:            .string          \"AAADDDDDabcd\"        ");
    parseSingleLine("str3:            .string          \"FFFFFFFFAAADDDDDabcd\"        ");
    /*
        printSymbolTable();
     */

    printf("Finished Successfully!\n");
    return 0;
}
