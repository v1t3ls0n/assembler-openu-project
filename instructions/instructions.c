#include "instructions.h"

Instruction *getInstructionByName(char *s)
{
    int i = 0;
    while (i < INST_SIZE)
    {
        if (strcmp(instructions[i]->keyword, s) == 0)
            return instructions[i];
        i++;
    }
    return NULL;
}

Bool isLabelNameLegal(char *s)
{
    int i = 0;
    while (i < 16)
    {
        if ((strcmp(regsName[i], s) == 0) || (strcmp(instructions[i]->keyword, s) == 0))
            return False;
        i++:
    }
    return True;
}
