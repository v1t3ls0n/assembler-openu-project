
#include "data.h"
#define MEMORY_START 100
/* Shared global State variables*/
extern State globalState;
extern Item *symbols[HASHSIZE];
extern Item *macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];
extern Word *convertNumberToWord(int n, EncodingFormat format);

unsigned static IC = MEMORY_START;
unsigned static DC = 0;
unsigned static ICF = 0;
unsigned static DCF = 0;

MemoryStack *codeMemoryStack;
MemoryStack *dataMemoryStack;

void addNumberToMemory(int number)
{
    Word *new;
    printf("inside addNumberToMemory\n");
    new = convertNumberToWord(number, Binary);
    writeToMemory(new, Data);
}

int writeToMemory(Word *word, DataType type)
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

void writeIntoCodeStack(Word *word)
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

void writeIntoDataStack(Word *word)
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

void increaseDataCounter(int amount)
{
    DC += amount;
    DC++;
}
void increaseInstructionCounter(int amount)
{
    IC += amount;
    IC++;
}

void printMemoryStacks(EncodingFormat format)
{

    Word *dataImgP = dataMemoryStack->head;

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

void resetCounters()
{

    ICF = IC;
    DCF = ICF + DC;
    DC = IC + 1;
    IC = MEMORY_START;
}