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
    printf("inside getOpIndex, s:%s\n", s);
    while (i < OP_SIZE)
    {
        printf("inside getOpIndex While loop, i:%d\n", i);

        if (strcmp(operations[i].keyword, s) == 0)
            return i;
        i++;
    }
    printf("inside getOpIndex end of function line 28\n");
    return -1;
}
