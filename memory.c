
#include "data.h"
#define MEMORY_START 100
/* Shared global State variables*/
extern State globalState;
extern Item *symbols[HASHSIZE];
extern Item *macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];

/* extern Word *convertNumberToWord(int n, EncodingFormat format);
 */
extern void updateFinalMemoryAddressesInSymbolTable();
extern BinaryWord *convertNumberToBinaryWord(int num);

unsigned static IC = MEMORY_START;
unsigned static DC = 0;
unsigned static ICF = 0;
unsigned static DCF = 0;

extern char *numToBin(int num);
static BinaryWord *binaryImg;

void initMemory()
{
    const int totalSize = DCF - MEMORY_START;
    int i, j;
    binaryImg = (BinaryWord *)malloc(totalSize * sizeof(BinaryWord));

    printf("inside initMemory\n");

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

void addWord(unsigned value, DataType type)
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
        printf("%d", binaryImg[index].digit[j].on ? 1 : 0);

    printf("\n");
}

void updateSymbolTableFinalValues()
{
    int i = 0;
    ICF = IC;
    DCF = ICF + DC;
    while (i < HASHSIZE && IC > 0)
    {
        if (symbols[i] != NULL)
            updateDataEntry(symbols[i]);

        i++;
    }
}

void updateDataEntry(Item *p)
{
    if (p->val.s.attrs.data)
    {
        p->val.s.value = p->val.s.value + DCF;
        p->val.s.offset = p->val.s.value % 16;
        p->val.s.base = p->val.s.value - p->val.s.offset;
        DC--;
    }

    if (p->next != NULL)
        updateDataEntry(p->next);
}

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

void updateFinalCountersValue()
{

    ICF = IC;
    DCF = ICF + DC;
    DC = IC;
    IC = MEMORY_START;
    printf("DC:%u IC:%u\nICF:%u DCF:%u\n", DC, IC, ICF, DCF);
    updateFinalMemoryAddressesInSymbolTable();
}
