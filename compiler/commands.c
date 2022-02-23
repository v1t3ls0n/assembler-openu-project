#include "data.h"
void main() { printf("\n\n\n\n\n\nTest somefile.c (in fake main function)\n\n\n\n\n\n"); }

Command *getCommandByName(char *s)
{
    int i = 0;

    while (i < CMD_AND_REGS_SIZE)
    {
        if (strcmp(commands[i].keyword, s) == 0)
            return &commands[i];
        i++;
    }
    return NULL; /*  */
}
