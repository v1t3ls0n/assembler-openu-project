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

FILE *ext;

void initExternalsFile(char *fileName)
{
    char *p, *name = calloc(strlen(fileName) + 3, sizeof(char *));
    strcat(name, fileName);
    p = strstr(name, ".am");
    p[2] = '\0';
    p[1] = '\0';
    strcat(name, "ext");
    printf("ext filename:%s\n", name);
    ext = fopen(name, "w+");
    if (ext == NULL)
    {
        printf("failed to create .ext compiled file\n");
        return;
    }
    free(name);
}

void writeToCurrentExternalsFile(char *name, unsigned base, unsigned offset)
{
    fprintf(ext, "%s BASE %d\n", name, base);
    fprintf(ext, "%s OFFSET %d\n", name, offset);
}

void closeExternalFile()
{
    if (ext)
        fclose(ext);
}