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
    if (s != NULL && *s)
        currentFileName = s;
    else
        currentFileName = 0;
}
char *getCurrentFileName()
{
    if (currentFileName && *currentFileName)
        return currentFileName;
    else
        return "";
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
