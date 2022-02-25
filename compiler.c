#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern Error currentError;
extern Item *symbols[HASHSIZE];
extern Item *macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];
extern HexWord *dec2Bin2sComplement(int n);

int main()
{
    int lineNumber = 1;
    if (!parseSingleLine("x:            .data          3        ", lineNumber))
    {
        globalState = collectErrors;
        yieldError(currentError, lineNumber);
        currentError = none;
    }

    return 0;
}

/*
    HexWord *singleWord[16];

        int lineNumber = 2;
        if (!parseSingleLine("x:          mov 3, r1", lineNumber))
        {
            globalState = collectErrors;
            yieldError(currentError, lineNumber);
            currentError = none;
        }

            printf("add:\n");
    addSymbol("GDGDS", 100, 1, 0, 1, 0);
    addSymbol("ABC", 108, 0, 1, 0, 0);
    addSymbol("GDGD", 112, 0, 0, 0, 1);
    printSymbolTable();

    singleWord[0] = dec2Bin2sComplement(-1);
    singleWord[1] = dec2Bin2sComplement(-7);
    singleWord[2] = dec2Bin2sComplement(-5);
    printObjectFile(singleWord, 16, 0);
    printf("add:\n");
    addSymbol("GDGDS", 100, 1, 0, 1, 0);
    addSymbol("ABC", 108, 0, 1, 0, 0);
    addSymbol("GDGD", 112, 0, 0, 0, 1);
    printSymbolTable();

     */
/*

Errors in parse.c:


*/

/*
dec2Bin2sComplement(-17);


 */

/*
    HexWord *singleWord[16];


    HexWord machineCodeObj[] = {
        {0x2, 0x1, 0x3, 0xF, 0x1},
        {0x4, 0x2, 0x3, 0x9, 0x1},
        {0x7, 0x8, 0x6, 0xD, 0x1},
        {0x3, 0x1, 0x5, 0xE, 0x1},
        {0x2, 0x4, 0x3, 0xC, 0x1},
        {0x4, 0x0, 0x3, 0xC, 0x3},
        {0x4, 0x0, 0x3, 0xB, 0x7},
        {0x4, 0x0, 0x3, 0xE, 0x2},
        {0x4, 0x0, 0x3, 0xA, 0x6},
    };
singleWord[0] = generateFirstWordEncodedHex(getOperationByName("mov"));
singleWord[1] = generateFirstWordEncodedHex(getOperationByName("cmp"));
singleWord[2] = generateFirstWordEncodedHex(getOperationByName("add"));
singleWord[3] = generateFirstWordEncodedHex(getOperationByName("sub"));
singleWord[4] = generateFirstWordEncodedHex(getOperationByName("lea"));
singleWord[5] = generateFirstWordEncodedHex(getOperationByName("clr"));
singleWord[6] = generateFirstWordEncodedHex(getOperationByName("not"));
singleWord[7] = generateFirstWordEncodedHex(getOperationByName("inc"));
singleWord[8] = generateFirstWordEncodedHex(getOperationByName("dec"));
singleWord[9] = generateFirstWordEncodedHex(getOperationByName("jmp"));
singleWord[10] = generateFirstWordEncodedHex(getOperationByName("bne"));
singleWord[11] = generateFirstWordEncodedHex(getOperationByName("jsr"));
singleWord[12] = generateFirstWordEncodedHex(getOperationByName("red"));
singleWord[13] = generateFirstWordEncodedHex(getOperationByName("prn"));
singleWord[14] = generateFirstWordEncodedHex(getOperationByName("rts"));
singleWord[15] = generateFirstWordEncodedHex(getOperationByName("stop"));
printObjectFile(singleWord, 16, 0);
printf("add:\n");
addSymbol("GDGDS", 100, 1, 0, 1, 0);
addSymbol("ABC", 108, 0, 1, 0, 0);
addSymbol("GDGD", 112, 0, 0, 0, 1);
printSymbolTable();







printObjectFile(singleWord, 1, 0);
printBinaryFile(machineCodeObj, 6, 3);

printObjectFile(machineCodeObj, 6, 3);
*/