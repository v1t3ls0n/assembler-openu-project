#include "data.h"

Command commands[] = {
    {0x0001, 0, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0002, 0, 1, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {0x0004, 10, 2, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0004, 11, 2, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0010, 0, 4, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {0x0020, 10, 5, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 11, 5, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 12, 5, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 13, 5, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0200, 10, 9, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 11, 9, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 12, 10, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x1000, 0, 12, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x2000, 0, 13, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {0x4000, 0, 14, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {0x8000, 0, 15, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};
Bool isLabelNameLegal(char *s);
Command *getCommandByName(char *s)
{
    int i = 0;
    int length = (int)(sizeof(commands) / sizeof(commands[0]));
    while (i < length)
    {
        if (strcmp(commands[i].keyword, s) == 0)
            return &commands[i];
        i++;
    }
    return NULL; /*  */
}

Bool isLabelNameLegal(char *s)
{
    int i = 0;
    const char *regs[] = {R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15};
    int length = (int)(sizeof(regs) / sizeof(regs[0]));

    /* if label name does not start with a alphabet letter */
    if (isalpha(s[0]) == 0)
        return False;

    /* maximum label name length is 31 characters */
    if (strlen(s) > MAX_LABEL_LEN)
        return False;

    while (i < length)
    {
        if ((strcmp(regs[i], s) == 0) || (strcmp(commands[i].keyword, s) == 0))
            return False;
        i++;
    }
    return True;
}

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
