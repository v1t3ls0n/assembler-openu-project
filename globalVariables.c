#include "data.h"

extern char *cloneString(char *s);
static State globalState = startProgram;
void setGlobalState(State new)
{
    globalState = new;
}
State getGlobalState()
{
    State current = globalState;
    return current;
}

static char *path;

void setFileNamePath(char *s)
{
    if (!*s)
        return;
    path = (char *)realloc(path, strlen(s) * sizeof(char));
    strcpy(path, cloneString(s));
}

char *getFileNamePath()
{
    return cloneString(path);
}

static unsigned currentLineNumber = 1;
void resetCurrentLineNumber()
{
    currentLineNumber = 1;
}
void increaseCurrentLineNumber()
{
    currentLineNumber++;
}
int getCurrentLineNumber()
{
    return currentLineNumber;
}
