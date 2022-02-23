#include "data.h"
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];

Operation *getOperationByName(char *s)
{
    int i = getOpIndex(s);
    return i != -1 ? &operations[i] : NULL;
}

Operation *getOperationByIndex(unsigned int i)
{
    return i < OP_SIZE ? &operations[i] : NULL;
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
