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

FILE *getSourceFilePointer()
{
    FILE *source;
    if ((source = fopen(strcat(getFileNamePath(), ".as"), "r")) == NULL)
    {
        yieldError(fileCouldNotBeOpened);
        return NULL;
    }
    return source;
}

void setFileNamePath(char *s)
{
    /*    size_t length; */
    printf("inside setFileNamePath, s:%s\n", s);
    if (!(*s))
    {
        memset(path, 0, strlen(path));
        return;
    }
    /*     length = strlen(s) - strlen(currentFileName); */
    path = (char *)realloc(path, strlen(s) * sizeof(char *));
    strcpy(path, cloneString(s));
}

char *getFileNamePath()
{
    /*     if (!(*s))
            return ""; */
    return cloneString(path);
}

void setCurrentFileName(char *s)
{
    printf("s:%s\n", s);
    currentFileName = (char *)realloc(currentFileName, strlen(s) * sizeof(char));
    strcpy(currentFileName, cloneString(s));
}

char *getCurrentFileName()
{
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
