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

void initAssemblyCodeFiles(char *fileName, assemblyCode *userCode)
{

    if ((userCode->src = fopen(strcat(cloneString(fileName), ".as"), "r")) == NULL)
    {
        yieldError(fileCouldNotBeOpened);
        return;
    }

    if ((userCode->expanded = fopen(strcat(cloneString(fileName), ".am"), "w+")) == NULL)
    {
        yieldError(fileCouldNotBeOpened);
        return;
    }
}

void setFileNamePath(char *s)
{
    /*    size_t length; */
    printf("inside setFileNamePath, s:%s\n", s);

    /*     length = strlen(s) - strlen(currentFileName); */

    path = (char *)realloc(path, strlen(s) * sizeof(char));
    strcpy(path, cloneString(s));
    printf("inside setFileNamePath, path:%s\n", path);
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
