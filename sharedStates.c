#include "data.h"

static State globalState = startProgram;
static char *path;
static unsigned currentLineNumber = 1;

void setGlobalState(State newState)
{
    globalState = newState;
}
State getGlobalState()
{
    State current = globalState;
    return current;
}

void setFileNamePath(char *s)
{
    if (!*s)
        return;
    path = (char *)realloc(path, strlen(s) * sizeof(char *));
    strcpy(path, cloneString(s));
}

char *getFileNamePath()
{
    return cloneString(path);
}

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
