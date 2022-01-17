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
    char regsName[] = {
        "r0",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7",
        "r8",
        "r9",
        "r10",
        "r11",
        "r12",
        "r13",
        "r14",
        "r15",
    };

    while (i < 16)
    {
        if ((strcmp(regsName[i], s) == 0) || (strcmp(instructions[i]->keyword, s) == 0))
            return False;
        i++:
    }
    return True;
}
