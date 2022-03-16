
#include "data.h"
extern void updateFinalMemoryAddressesInSymbolTable();
unsigned static IC = MEMORY_START;
unsigned static DC = 0;
unsigned static ICF = 0;
unsigned static DCF = 0;
extern char *numToBin(int num);
static BinaryWord *binaryImg;
unsigned getDC() { return DC; }
unsigned getIC() { return IC; }
unsigned getICF() { return ICF; }
unsigned getDCF() { return DCF; }
void increaseDataCounter(int amount)
{
    DC += amount;
}
void increaseInstructionCounter(int amount)
{
    IC += amount;
}
void initMemory()
{
    const int totalSize = DCF - MEMORY_START;
    int i, j;
    binaryImg = (BinaryWord *)malloc(totalSize * sizeof(BinaryWord));
    /*
        printf("inside initMemory\n");
     */
    for (i = 0; i < totalSize; i++)
    {
        for (j = 0; j < BINARY_WORD_SIZE; j++)
        {

            binaryImg[i].digit[j].on = 0;
        }
    }
}
void printBinaryImg()
{
    int i;
    int totalSize = DCF - MEMORY_START;
    for (i = 0; i < totalSize; i++)
    {
        printf("%04d ", MEMORY_START + i);
        printWordBinary(i);
    }
}
void addWord(int value, DataType type)
{
    if (type == Code)
        addWordToCodeImage(numToBin(value));
    else if (type == Data)
        addWordToDataImage(numToBin(value));
}
void addWordToDataImage(char *s)
{
    wordStringToWordObj(s, Data);
    DC++;
}
void addWordToCodeImage(char *s)
{
    /*     printf("inside addWordToCodeImage, s:%s\n", s);
     */
    wordStringToWordObj(s, Code);
    IC++;
}
void wordStringToWordObj(char *s, DataType type)
{
    int j;
    int index = type == Code ? IC - MEMORY_START : DC - MEMORY_START;
    for (j = 0; j < BINARY_WORD_SIZE; j++)
        binaryImg[index].digit[j].on = s[j] == '1' ? 1 : 0;
}

void wordObjToBinStr(BinaryWord *word, char *s)
{
    int j = 0;

    for (j = 0; j < BINARY_WORD_SIZE; j++)
        strcat(s, word->digit[j].on ? "1" : "0");
}

void printWordBinary(unsigned index)
{
    int j;
    for (j = 0; j < BINARY_WORD_SIZE; j++)
    {
        if (j % 4 == 0)
            printf(" ");
        printf("%d", binaryImg[index].digit[j].on ? 1 : 0);
    }

    printf("\n");
}
void updateFinalCountersValue()
{

    ICF = IC;
    DCF = ICF + DC;
    DC = IC;
    IC = MEMORY_START;
    printf("DC:%u IC:%u\nICF:%u DCF:%u\n", DC, IC, ICF, DCF);
    updateFinalMemoryAddressesInSymbolTable();
}

void printMemoryImgInRequiredObjFileFormat()
{
    int i;
    int totalSize = DCF - MEMORY_START;
    char wordStr[BINARY_WORD_SIZE] = {0};
    char a[4] = {0}, b[4] = {0}, c[4] = {0}, d[4] = {0}, e[4] = {0};
    printf("              %d   %d\n", ICF, DCF);
    for (i = 0; i < totalSize; i++)
    {
        wordObjToBinStr(&binaryImg[i], wordStr);
        memcpy(a, &wordStr[0], 4);
        memcpy(b, &wordStr[4], 4);
        memcpy(c, &wordStr[8], 4);
        memcpy(d, &wordStr[12], 4);
        memcpy(e, &wordStr[16], 4);
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", MEMORY_START + i, binary2Decimal(a), binary2Decimal(b), binary2Decimal(c), binary2Decimal(d), binary2Decimal(e));
        memset(wordStr, 0, BINARY_WORD_SIZE);
        memset(a, 0, 4);
        memset(b, 0, 4);
        memset(c, 0, 4);
        memset(d, 0, 4);
        memset(e, 0, 4);
    }
}
