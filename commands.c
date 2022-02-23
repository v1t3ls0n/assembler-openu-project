#include "data.h"
/* Complex Struct Constant Variables: */
extern Command commands[];

Command *getCommandByName(char *s)
{
    int i = 0;

    while (i < CMD_AND_REGS_SIZE)
    {
        if (strcmp(commands[i].keyword, s) == 0)
            return &commands[i];
        i++;
    }
    return NULL;
}
