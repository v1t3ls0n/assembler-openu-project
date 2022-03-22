#include "data.h"

static State globalState = startProgram;
void updateGlobalState(State new)
{
    globalState = new;
}
State getGlobalState()
{
    State current = globalState;
    return current;
}

static char *currentFileName;
void setCurrentFileName(char *s)
{
    if (!*s)
        return;
    currentFileName = (char *)realloc(currentFileName, strlen(s) * sizeof(char));
    strcpy(currentFileName, s);
}

char *getCurrentFileName()
{
    if (currentFileName && *currentFileName)
    {
        char *copy = calloc(strlen(currentFileName), sizeof(char));
        strcpy(copy, currentFileName);
        return copy;
    }
    else
        return NULL;
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
