#include "data.h"
/* Shared global State variables*/
extern Operation operations[OP_SIZE];
extern char *decToHex(int num);
extern char *hexToBin(char *hex);
extern void printBinaryFile(HexWord *words[], unsigned int ICF, unsigned int DCF);

char *generateFirstWordEncodedToBinary(Operation *operation)
{
    return strcat(hexToBin(decToHex(A)), hexToBin(decToHex(operation->op)));
}

HexWord *generateFirstWordEncodedHex(Operation *operation)
{
    char *buf = (char *)calloc(6, sizeof(char));
    HexWord *newWord = (HexWord *)malloc(1 * sizeof(HexWord *));
    sprintf(buf, "%x", (A | operation->op));
    newWord->_A = buf[0];
    newWord->_B = buf[1];
    newWord->_C = buf[2];
    newWord->_D = buf[3];
    newWord->_E = buf[4];
    free(buf);
    return newWord;
}

HexWord *encodeIntNum(int num)
{
    char *buf;
    HexWord *newWord;
    newWord = (HexWord *)malloc(1 * sizeof(HexWord *));
    buf = decToHex(num);
    newWord->_A = buf[0] ? hex2int(buf[0]) : '0';
    newWord->_B = buf[1] ? hex2int(buf[1]) : '0';
    newWord->_C = buf[2] ? hex2int(buf[2]) : '0';
    newWord->_D = buf[3] ? hex2int(buf[3]) : '0';
    newWord->_E = buf[4] ? hex2int(buf[4]) : '0';
    free(buf);
    return newWord;
}

void printObjectFile(HexWord *words[], unsigned int ICF, unsigned int DCF)
{
    int i;
    printf("              %d   %d\n", ICF, DCF);
    for (i = 0; i < (DCF + ICF) - 1; i++) /*  */
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", 100 + i, words[i]->_A, words[i]->_B, words[i]->_C, words[i]->_D, words[i]->_E);
}

void printBinaryFile(HexWord *words[], unsigned int ICF, unsigned int DCF)
{
    int i;
    for (i = 0; i < (DCF + ICF); i++)
    {
        printf("%04d ", 100 + i);
        printf("%s", hexToBin(decToHex(words[i]->_A)));
        printf("%s", hexToBin(decToHex(words[i]->_B)));
        printf("%s", hexToBin(decToHex(words[i]->_C)));
        printf("%s", hexToBin(decToHex(words[i]->_D)));
        printf("%s\n", hexToBin(decToHex(words[i]->_E)));
    }
}