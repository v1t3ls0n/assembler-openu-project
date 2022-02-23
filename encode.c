#include "data.h"
/* Shared global State variables*/

extern char *decToHex(int num);
extern char *hexToBin(char *hex);
extern void printBinaryFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF);

char *generateFirstWordEncodedToBinary(Command *cmd)
{
    return strcat(hexToBin(decToHex(A)), hexToBin(decToHex(cmd->op)));
}

EncodedWord *generateFirstWordEncodedHex(Command *cmd)
{
    char *buf = (char *)calloc(6, sizeof(char));
    EncodedWord *newWord = (EncodedWord *)malloc(1 * sizeof(EncodedWord *));
    sprintf(buf, "%x", (A | cmd->op));
    newWord->_A = buf[0];
    newWord->_B = buf[1];
    newWord->_C = buf[2];
    newWord->_D = buf[3];
    newWord->_E = buf[4];
    free(buf);
    return newWord;
}
EncodedWord *encodeIntNum(int num)
{
    char *buf;
    EncodedWord *newWord;
    newWord = (EncodedWord *)malloc(1 * sizeof(EncodedWord *));
    buf = decToHex(num);
    newWord->_A = buf[0] ? hex2int(buf[0]) : '0';
    newWord->_B = buf[1] ? hex2int(buf[1]) : '0';
    newWord->_C = buf[2] ? hex2int(buf[2]) : '0';
    newWord->_D = buf[3] ? hex2int(buf[3]) : '0';
    newWord->_E = buf[4] ? hex2int(buf[4]) : '0';
    free(buf);
    return newWord;
}

void printObjectFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF)
{
    int i;
    printf("              %d   %d\n", ICF, DCF);
    for (i = 0; i < (DCF + ICF) - 1; i++) /*  */
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", 100 + i, words[i]->_A, words[i]->_B, words[i]->_C, words[i]->_D, words[i]->_E);
}

void printBinaryFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF)
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