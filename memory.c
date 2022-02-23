
#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern Error currentError;
extern Item *symbols[HASHSIZE];
extern Item *macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];

unsigned static IC = 0;
unsigned static DC = 0;
unsigned static ICF = 100;
unsigned static DCF = 101;

Word *_data;
Word *_code;

int writeToMemory(EncodedWord value, DataType type)
{
    Word *newEntry = (Word *)malloc(sizeof(Word *));
    unsigned address = type == Code ? IC : DCF;

    if (!newEntry || (DC + IC > 8191))
    {
        globalState = collectErrors;
        currentError = memoryAllocationFailure;
        return 0;
    }

    newEntry->value = value;
    newEntry->next = NULL;

    if (type == Code)
    {
        newEntry->address = IC;
        _code->next = newEntry;
        IC++;
        ICF += IC;
    }
    else if (type == Data)
    {
        newEntry->address = DC;
        _data->next = newEntry;
        DC++;
        DCF = ICF + DC;
    }

    return address;
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
