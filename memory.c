
#include "data.h"
extern void updateFinalMemoryAddressesInSymbolTable();
extern HexWord *convertBinaryWordToHex(BinaryWord *word);
extern char *numToBin(int num);
extern char *decToHex(int num);

static BinaryWord *binaryImg;
static HexWord *hexImg;
void covertBinaryMemoryImageToHexImageForObFile();
unsigned static IC = MEMORY_START;
unsigned static DC = 0;
unsigned static ICF = 0;
unsigned static DCF = 0;
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
    for (i = 0; i < totalSize; i++)
    {
        for (j = 0; j < BINARY_WORD_SIZE; j++)
        {
            binaryImg[i].digit[j].on = 0;
        }
    }
}

void resetMemory()
{
    free(binaryImg);
    free(hexImg);
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
    for (j = 0; j < BINARY_WORD_SIZE + 5; j++)
    {
        if (j > 0 && j % 4 == 0)
            strcat(s, " ");

        else
            strcat(s, word->digit[j].on ? "1" : "0");
    }
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
    /*     printf("DC:%u IC:%u\nICF:%u DCF:%u\n", DC, IC, ICF, DCF); */
    updateFinalMemoryAddressesInSymbolTable();
}

void covertBinaryMemoryImageToHexImageForObFile()
{
    int i;
    int totalSize = DCF - MEMORY_START;
    hexImg = (HexWord *)malloc(totalSize * sizeof(HexWord));
    for (i = 0; i < totalSize; i++)
        hexImg[i] = *convertBinaryWordToHex(&binaryImg[i]);
}

void printMemoryImgInRequiredObjFileFormat()
{

    int i;
    int totalSize = DCF - MEMORY_START;
    hexImg = (HexWord *)malloc(totalSize * sizeof(HexWord));
    printf("%d %d\n", ICF - MEMORY_START, DCF - ICF);
    for (i = 0; i < totalSize; i++)
    {
        hexImg[i] = *convertBinaryWordToHex(&binaryImg[i]);
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", MEMORY_START + i, hexImg[i]._A, hexImg[i]._B, hexImg[i]._C, hexImg[i]._D, hexImg[i]._E);
    }
}
