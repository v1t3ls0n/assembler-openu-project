
#include "data.h"
#define MEMORY_START 100
/* Shared global State variables*/
extern State globalState;
extern Error currentError;
extern Item *symbols[HASHSIZE];
extern Item *macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];

unsigned static IC = MEMORY_START;
unsigned static DC = MEMORY_START + 1;
unsigned static ICF = MEMORY_START;
unsigned static DCF = MEMORY_START + 1;

MemoryStack *codeMemoryStack, *dataMemoryStack;

int writeToMemory(Word *word, DataType type)
{

    if (DC + IC > 8191)
    {
        globalState = collectErrors;
        currentError = memoryAllocationFailure;
        return False;
    }

    if (type == Code)
        writeIntoCodeStack(word);

    else
        writeIntoDataStack(word);

    return type == Code ? IC : DC;
}

void writeIntoCodeStack(Word *word)
{

    if (codeMemoryStack->head == NULL)
    {
        codeMemoryStack->head = word;
        codeMemoryStack->tail = word;
    }
    else
        codeMemoryStack->tail = word;

    IC++;
}

void writeIntoDataStack(Word *word)
{
    if (dataMemoryStack->head == NULL)
    {
        dataMemoryStack->head = word;
        dataMemoryStack->tail = word;
    }
    else
        dataMemoryStack->tail = word;

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

void increaseDataCounter(int amount)
{
    DC += amount;
}
void inceaseInstructionCounter(int amount)
{
    ICF = IC += amount;
}

void resetCounters()
{
    ICF = IC;
    DCF = ICF + 1;
    IC = MEMORY_START;
}