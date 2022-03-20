
#include "data.h"
extern void updateFinalMemoryAddressesInSymbolTable();
unsigned static IC = MEMORY_START;
unsigned static DC = 0;
unsigned static ICF = 0;
unsigned static DCF = 0;

extern char *numToBin(int num);
extern char *decToHex(int num);

static BinaryWord *binaryImg;
HexWord *convertBinaryWordToRequiredObjWordFormat(BinaryWord *word);

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

void printMemoryImgInRequiredObjFileFormat()
{

    int i;
    int totalSize = DCF - MEMORY_START;
    HexWord *p;
    printf("%d %d\n", ICF - MEMORY_START, DCF - ICF);

    for (i = 0; i < totalSize; i++)
    {
        p = convertBinaryWordToRequiredObjWordFormat(&binaryImg[i]);
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", MEMORY_START + i, p->_A, p->_B, p->_C, p->_D, p->_E);
        free(p);
    }
}

HexWord *convertBinaryWordToRequiredObjWordFormat(BinaryWord *word)
{
    int i = 0;
    char hexDigits[4] = {0};
    HexWord *newHex = (HexWord *)malloc(sizeof(HexWord));
    for (i = BINARY_WORD_SIZE - 1; i >= 0; i--)
    {
        hexDigits[i % 4] = word->digit[i].on ? '1' : '0';
        if (i % 4 == 0)
        {
            switch (i)
            {
            case 16:
                newHex->_A = binaryStringToHexNumber(hexDigits);
                break;
            case 12:
                newHex->_B = binaryStringToHexNumber(hexDigits);
                break;
            case 8:
                newHex->_C = binaryStringToHexNumber(hexDigits);
                break;
            case 4:
                newHex->_D = binaryStringToHexNumber(hexDigits);
                break;
            case 0:
                newHex->_E = binaryStringToHexNumber(hexDigits);
                break;
            default:
                break;
            }

            memset(hexDigits, 0, 4);
        }
    }

    return newHex;
}