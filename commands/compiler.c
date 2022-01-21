
#include "data.h"
State globalState = startProgram;
Error currentError = noErrors;

int main()
{
    EncodedWord *singleWord[16];

    /*
    EncodedWord *singleWord;

        EncodedWord machineCodeObj[] = {
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

    singleWord = encodeIntNum(3);

     */
    singleWord[0] = generateFirstWordEncodedHex(getCommandByName("mov"));
    singleWord[1] = generateFirstWordEncodedHex(getCommandByName("cmp"));
    singleWord[2] = generateFirstWordEncodedHex(getCommandByName("add"));
    singleWord[3] = generateFirstWordEncodedHex(getCommandByName("sub"));
    singleWord[4] = generateFirstWordEncodedHex(getCommandByName("lea"));
    singleWord[5] = generateFirstWordEncodedHex(getCommandByName("clr"));
    singleWord[6] = generateFirstWordEncodedHex(getCommandByName("not"));
    singleWord[7] = generateFirstWordEncodedHex(getCommandByName("inc"));
    singleWord[8] = generateFirstWordEncodedHex(getCommandByName("dec"));
    singleWord[9] = generateFirstWordEncodedHex(getCommandByName("jmp"));
    singleWord[10] = generateFirstWordEncodedHex(getCommandByName("bne"));
    singleWord[11] = generateFirstWordEncodedHex(getCommandByName("jsr"));
    singleWord[12] = generateFirstWordEncodedHex(getCommandByName("red"));
    singleWord[13] = generateFirstWordEncodedHex(getCommandByName("prn"));
    singleWord[14] = generateFirstWordEncodedHex(getCommandByName("rts"));
    singleWord[15] = generateFirstWordEncodedHex(getCommandByName("stop"));
    printf("add:\n");
    printObjectFile(singleWord, 16, 0);

    addSymbol("GDGDS", 100, 1, 0, 1, 0);
    addSymbol("ABC", 108, 0, 1, 0, 0);
    addSymbol("GDGD", 112, 0, 0, 0, 1);
    /*
    printSymbolTable();
    printObjectFile(singleWord, 1, 0);

    printObjectFile(machineCodeObj, 6, 3);
    printBinaryFile(machineCodeObj, 6, 3);
*/
    return 0;
}
