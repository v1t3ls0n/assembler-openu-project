#include "data.h"
extern Command commands[];
void printObjectFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF);
void printBinaryFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF);
EncodedWord *encodeIntNum(int num);
char *generateFirstWordEncodedToBinary(Command *cmd);
EncodedWord *generateFirstWordEncodedHex(Command *cmd);

char *generateFirstWordEncodedToBinary(Command *cmd)
{
    return strcat(hexToBin(decToHex(A)), hexToBin(decToHex(cmd->opMachineCodeHex)));
}

EncodedWord *generateFirstWordEncodedHex(Command *cmd)
{
    char *buf = (char *)calloc(6, sizeof(char));
    EncodedWord *newWord = (EncodedWord *)malloc(1 * sizeof(EncodedWord *));
    sprintf(buf, "%x", (A | cmd->opMachineCodeHex));
    newWord->POS_A = buf[0];
    newWord->POS_B = buf[1];
    newWord->POS_C = buf[2];
    newWord->POS_D = buf[3];
    newWord->POS_E = buf[4];
    free(buf);
    return newWord;
}
EncodedWord *encodeIntNum(int num)
{
    char *buf;
    EncodedWord *newWord;
    newWord = (EncodedWord *)malloc(1 * sizeof(EncodedWord *));
    buf = decToHex(num);
    newWord->POS_A = buf[0] ? hex2int(buf[0]) : '0';
    newWord->POS_B = buf[1] ? hex2int(buf[1]) : '0';
    newWord->POS_C = buf[2] ? hex2int(buf[2]) : '0';
    newWord->POS_D = buf[3] ? hex2int(buf[3]) : '0';
    newWord->POS_E = buf[4] ? hex2int(buf[4]) : '0';
    free(buf);
    return newWord;
}

void printObjectFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF)
{
    int i;
    printf("              %d   %d\n", ICF, DCF);
    for (i = 0; i < (DCF + ICF); i++) /*  */
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", 100 + i, words[i]->POS_A, words[i]->POS_B, words[i]->POS_C, words[i]->POS_D, words[i]->POS_E);
}

void printBinaryFile(EncodedWord *words[], unsigned int ICF, unsigned int DCF)
{
    int i;
    for (i = 0; i < (DCF + ICF); i++)
    {
        printf("%04d ", 100 + i);
        printf("%s", hexToBin(decToHex(words[i]->POS_A)));
        printf("%s", hexToBin(decToHex(words[i]->POS_B)));
        printf("%s", hexToBin(decToHex(words[i]->POS_C)));
        printf("%s", hexToBin(decToHex(words[i]->POS_D)));
        printf("%s\n", hexToBin(decToHex(words[i]->POS_E)));
    }
}