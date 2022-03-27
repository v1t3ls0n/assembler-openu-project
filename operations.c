#include "data.h"

static Operation operations[OP_SIZE] = {
    {0x0001, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0002, 0, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {0x0004, 10, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0004, 11, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0010, 0, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {0x0020, 10, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 11, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 12, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 13, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0200, 10, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 11, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 12, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x1000, 0, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x2000, 0, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {0x4000, 0, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {0x8000, 0, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};

Bool isOperationName(char *s)
{
    return getOpIndex(s) != -1 ? True : False;
}

Operation *getOperationByName(char *s)
{
    int i = getOpIndex(s);

    if (i != -1)
        return &operations[i];

    return NULL;
}

Operation *getOperationByIndex(unsigned int i)
{
    return i < OP_SIZE ? &operations[i] : NULL;
}

char *getOperationName(char *s)
{
    int i = 0;
    while (i < OP_SIZE)
    {

        if (strstr(operations[i].keyword, s) != NULL)
            return operations[i].keyword;
        i++;
    }
    return 0;
}

int getOpIndex(char *s)
{
    int i = 0;
    while (i < OP_SIZE)
    {

        if (strcmp(operations[i].keyword, s) == 0)
            return i;
        i++;
    }
    return -1;
}
Bool isOperationNotStrict(char *s)
{
    int i = 0;
    while (i < OP_SIZE)
    {

        if (strstr(operations[i].keyword, s) != NULL)
            return True;
        i++;
    }
    return False;
}