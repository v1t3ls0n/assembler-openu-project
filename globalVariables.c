#include "data.h"

extern char *cloneString(char *s);
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

void setCurrentFileName(char *s)
{
    if (!*s)
        return;
    currentFileName = (char *)realloc(currentFileName, strlen(s) * sizeof(char));
    strcpy(currentFileName, cloneString(s));
}

char *getCurrentFileName()
{
    if (!*currentFileName)
        return "";
    return cloneString(currentFileName);
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
