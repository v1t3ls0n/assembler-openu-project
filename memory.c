
#include "data.h"
#define MEMORY_START 100
/* Shared global State variables*/
extern State globalState;
extern Item* symbols[HASHSIZE];
extern Item* macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];

/* extern Word *convertNumberToWord(int n, EncodingFormat format);
 */
extern void updateFinalMemoryAddressesInSymbolTable();
extern BinaryWord* convertNumberToBinaryWord(int num);

unsigned static IC = MEMORY_START;
unsigned static DC = 0;
unsigned static ICF = 0;
unsigned static DCF = 0;

extern char* numToBin(int num);



static BinaryWord* binaryImg;

void initMemory()
{
    const int totalSize = DCF - MEMORY_START;
    int i, j;
    binaryImg = (BinaryWord*)malloc(totalSize * sizeof(BinaryWord));


    printf("inside initMemory\n");

    for (i = 0; i < totalSize; i++)
    {
        for (j = 0; j < BINARY_WORD_SIZE; j++)
        {

            binaryImg[i].digit[j].on = 0;
        }
    }

    /*
        printf("inside initMemory,totalSize:%d\nbinaryImg size:%d\n", totalSize, (int)(sizeof(binaryImg) * sizeof(BinaryWord*) * 20));
     */

}
void printBinaryImg()
{
    int i;
    int totalSize = DCF - MEMORY_START;
    for (i = 0; i < totalSize; i++)
        printWordBinary(i);

}

void writeIntegerIntoDataMemoryBinaryImg(int number)
{
    printf("inside write writeIntegerIntoDataMemoryBinaryImg, number:%d Binary:", number);

    /* int index = DC - MEMORY_START;
    binaryImg[index] = *convertNumberToBinaryWord(number);
    DC++;
    printWordBinary(index);
    printf("\n");
     */
}

void addWord(unsigned value, DataType type)
{
    if (type == Code)
        addWordToCodeImage(numToBin(value));
    else if (type == Data)
        addWordToDataImage(numToBin(value));

}


void addWordToDataImage(char* s)
{

    wordStringToWordObj(s, Data);
    DC++;
}

void addWordToCodeImage(char* s)
{
    wordStringToWordObj(s, Code);
    IC++;
}

void wordStringToWordObj(char* s, DataType type)
{
    int j;
    int index = type == IC ? IC - MEMORY_START : DC - MEMORY_START;
    for (j = 0; j < BINARY_WORD_SIZE; j++)
        binaryImg[index].digit[j].on = s[j] == '1' ? 1 : 0;


    printWordBinary(index);

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

void updateDataEntry(Item* p)
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

/*

MemoryStack* codeMemoryStack;
MemoryStack* dataMemoryStack;
void printMemoryStacks(EncodingFormat format)
{

    Word* dataImgP = dataMemoryStack->head;

    if (format == Binary)
    {
        int i = 0;
        printf("Data Image Binary:\n");
        while (dataImgP != NULL)
        {
            while (i < BINARY_WORD_SIZE)
            {
                printf("%c", dataImgP->value->binary->digit[i].on ? '1' : '0');
            }
            printf("\n");
            dataImgP = dataImgP->next;
        }
    }
}


int writeToMemory(Word* word, DataType type)
{
    printf("inside writeToMemory\n");

    if (DC + IC > 8191)
    {
        yieldError(memoryAllocationFailure);
        return Err;
    }

    if (type == Code)
        writeIntoCodeStack(word);

    else
        writeIntoDataStack(word);

    return type == Code ? IC : DC;
}

void writeIntoCodeStack(Word* word)
{

      if (codeMemoryStack == NULL)
        {
            codeMemoryStack = calloc(1, sizeof(MemoryStack *));
            codeMemoryStack->head = word;
            codeMemoryStack->tail = word;
        }
        else
        {
            codeMemoryStack->tail->next = word;
            codeMemoryStack->tail = word;
        }

    IC++;
}

 void writeIntoDataStack(Word* word)
{
        printf("inside write into data stack\n");
        if (dataMemoryStack == NULL)
        {
            printf("in line 71\n");

            dataMemoryStack->head = calloc(1, sizeof(Word *));
            dataMemoryStack->tail = calloc(1, sizeof(Word *));

            dataMemoryStack->head = word;
            dataMemoryStack->tail = word;
        }
        else
        {
            dataMemoryStack->tail->next = calloc(1, sizeof(Word *));
            dataMemoryStack->tail->next = &word;
            dataMemoryStack->tail = word;
        }

    DC++;
}


 */