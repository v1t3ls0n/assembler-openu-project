typedef struct
{
    HexWord hex;
    BinaryWord binary;
} Word;

/*
#include "data.h"

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
    char *buf = (char *)(6, sizeof(char*));
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
*/
/* void wordObjToBinStr(BinaryWord *word, char *s)
{
    int j = 0;
    for (j = 0; j < BINARY_WORD_SIZE + 5; j++)
    {
        if (j > 0 && j % 4 == 0)
            strcat(s, " ");

        else
            strcat(s, word->digit[j].on ? "1" : "0");
    }
}
 */
/* Word *convertNumberToWord(int n, EncodingFormat format)
{
    Word *w = (Word *)malloc(sizeof(Word *));
    if (n < 0)
        n = dec2Bin2sComplement(n);

    if (format == Binary)
        w->value->binary = convertNumberToBinaryWord(n);
    if (format == Hexadecimal)
        w->value->hex = convertNumToHexWord(n);

    w->next = NULL;

    printf("inside convert number to word last line\n");
    return w;
}
 */
BinaryWord *convertNumberToBinaryWord(int num)
{
    BinaryWord *newBinary = (BinaryWord *)malloc(sizeof(BinaryWord *));
    char *buf = hexToBin(decToHex(num));
    int i = BINARY_WORD_SIZE - 1;
    int j = strlen(buf);

    /* printf("inside convertNumberToBinaryWord, binary is:%s\n", buf);
     */
    while (j > 0)
    {
        newBinary->digit[i].on = buf[j] == '1' ? 1 : 0;
        j--;
        i++;
    }
    free(buf);
    return newBinary;
}
HexWord *convertNumToHexWord(int num)
{
    int i = 0;
    char *buf = decToHex(num);
    HexWord *newHex = (HexWord *)malloc(sizeof(HexWord *));

    /*     printf("inside convertNumToHexWord, buf:%s\n", buf);
     */
    while (i < 5)
    {
        if (i == 0)
            newHex->_A = i < strlen(buf) ? hex2int(buf[i]) : 0;
        if (i == 1)
            newHex->_B = i < strlen(buf) ? hex2int(buf[i]) : 0;
        if (i == 2)
            newHex->_C = i < strlen(buf) ? hex2int(buf[i]) : 0;
        if (i == 3)
            newHex->_D = i < strlen(buf) ? hex2int(buf[i]) : 0;
        if (i == 4)
            newHex->_E = i < strlen(buf) ? hex2int(buf[i]) : 0;
        i++;
    }
    free(buf);
    return newHex;
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