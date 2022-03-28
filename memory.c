
#include "data.h"
extern HexWord *convertBinaryWordToHex(BinaryWord *word);
extern char *numToBin(int num);

void covertBinaryMemoryImageToHexImageForObFile();
static BinaryWord *binaryImg = NULL;
static HexWord *hexImg = NULL;
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
void allocMemoryImg()
{
    const int totalSize = DCF - MEMORY_START;
    int i, j;
    binaryImg = (BinaryWord *)realloc(binaryImg, totalSize * sizeof(BinaryWord));
    hexImg = (HexWord *)realloc(binaryImg, totalSize * sizeof(HexWord));
    for (i = 0; i < totalSize; i++)
    {
        hexImg[i]._A = 0;
        hexImg[i]._B = 0;
        hexImg[i]._C = 0;
        hexImg[i]._D = 0;
        hexImg[i]._E = 0;

        for (j = 0; j < BINARY_WORD_SIZE; j++)
        {
            binaryImg[i].digit[j].on = 0;
        }
    }
}

void resetMemoryCounters()
{
    IC = MEMORY_START;
    DC = 0;
    ICF = 0;
    DCF = 0;
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
void calcFinalAddrsCountersValues()
{

    ICF = IC;
    DCF = ICF + DC;
    DC = IC;
    IC = MEMORY_START;
    printf("DC:%u IC:%u\nICF:%u DCF:%u\n", DC, IC, ICF, DCF);
}

void printMemoryImgInRequiredObjFileFormat()
{
    extern BinaryWord *binaryImg;
    extern HexWord *hexImg;
    int i;
    int totalSize = DCF - MEMORY_START;
    printf("%d %d\n", ICF - MEMORY_START, DCF - ICF);
    for (i = 0; i < totalSize; i++)
    {
        hexImg[i] = *convertBinaryWordToHex(&binaryImg[i]);
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", MEMORY_START + i, hexImg[i]._A, hexImg[i]._B, hexImg[i]._C, hexImg[i]._D, hexImg[i]._E);
    }
}

void writeMemoryImageToObFile(FILE *fp)
{
    extern BinaryWord *binaryImg;
    extern HexWord *hexImg;
    int i;
    int totalSize = DCF - MEMORY_START;
    fprintf(fp, "%d %d\n", ICF - MEMORY_START, DCF - ICF);
    for (i = 0; i < totalSize; i++)
    {
        hexImg[i] = *convertBinaryWordToHex(&binaryImg[i]);
        fprintf(fp, "%04d A%x-B%x-C%x-D%x-E%x\n", MEMORY_START + i, hexImg[i]._A, hexImg[i]._B, hexImg[i]._C, hexImg[i]._D, hexImg[i]._E);
    }
}

char *numToBin(int num)
{
    int i = 0;
    unsigned int result;
    char *word, hex[6];
    word = (char *)calloc(BINARY_WORD_SIZE + 1, sizeof(char *));
    if (num < 0)
    {
        result = abs(num);
        result = ~result;
        result++;
        sprintf(hex, "%05x", (int)(result & 0x4ffff));
    }
    else
        sprintf(hex, "%05x", (int)num & 0xfffff);

    while (hex[i] != '\0')
    {
        switch (hex[i])
        {

        case '0':
            strcat(word, "0000");
            break;
        case '1':
            strcat(word, "0001");
            break;
        case '2':
            strcat(word, "0010");
            break;
        case '3':
            strcat(word, "0011");
            break;
        case '4':
            strcat(word, "0100");
            break;
        case '5':
            strcat(word, "0101");
            break;
        case '6':
            strcat(word, "0110");
            break;
        case '7':
            strcat(word, "0111");
            break;
        case '8':
            strcat(word, "1000");
            break;
        case '9':
            strcat(word, "1001");
            break;
        case 'A':
        case 'a':
            strcat(word, "1010");
            break;
        case 'B':
        case 'b':
            strcat(word, "1011");
            break;
        case 'C':
        case 'c':
            strcat(word, "1100");
            break;
        case 'D':
        case 'd':
            strcat(word, "1101");
            break;
        case 'E':
        case 'e':
            strcat(word, "1110");
            break;
        case 'F':
        case 'f':
            strcat(word, "1111");
            break;
        default:
            break;
        }

        i++;
    }

    strcat(word, "\0");
    return word;
}
HexWord *convertBinaryWordToHex(BinaryWord *word)
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
                newHex->_E = binaryStringToHexNumber(hexDigits);
                break;
            case 12:
                newHex->_D = binaryStringToHexNumber(hexDigits);
                break;
            case 8:
                newHex->_C = binaryStringToHexNumber(hexDigits);
                break;
            case 4:
                newHex->_B = binaryStringToHexNumber(hexDigits);
                break;
            case 0:
                newHex->_A = binaryStringToHexNumber(hexDigits);
                break;
            default:
                break;
            }

            memset(hexDigits, 0, 4);
        }
    }

    return newHex;
}
unsigned binaryStringToHexNumber(char binaryStr[4])
{

    if (!strcmp(binaryStr, "0000"))
        return 0;
    if (!strcmp(binaryStr, "0001"))
        return 1;
    if (!strcmp(binaryStr, "0010"))
        return 2;
    if (!strcmp(binaryStr, "0011"))
        return 3;
    if (!strcmp(binaryStr, "0100"))
        return 4;
    if (!strcmp(binaryStr, "0101"))
        return 5;
    if (!strcmp(binaryStr, "0110"))
        return 6;
    if (!strcmp(binaryStr, "0111"))
        return 7;
    if (!strcmp(binaryStr, "1000"))
        return 8;
    if (!strcmp(binaryStr, "1001"))
        return 9;
    if (!strcmp(binaryStr, "1010"))
        return 0xA;
    if (!strcmp(binaryStr, "1011"))
        return 0xB;
    if (!strcmp(binaryStr, "1100"))
        return 0xC;
    if (!strcmp(binaryStr, "1101"))
        return 0xD;
    if (!strcmp(binaryStr, "1110"))
        return 0xE;
    if (!strcmp(binaryStr, "1111"))
        return 0xF;

    return 0;
}