#include "data.h"

extern void writeMemoryImageToObFile(FILE *fp);
extern Bool writeEntriesToFile(FILE *fp);
extern Bool areEntriesExist();
extern Bool areExternalsExist();
void generateObFile(char *baseFileName);
void createEntriesFile(char *baseFileName);

void exportFilesMainHandler(char *baseFileName)
{
    generateObFile(baseFileName);
    if (areEntriesExist())
        createEntriesFile(baseFileName);
}

void generateObFile(char *baseFileName)
{
    FILE *ob;
    char *fileName = calloc(strlen(baseFileName) + 3, sizeof(char *));
    sscanf(baseFileName, "%s", fileName);
    strcat(fileName, ".ob");
    ob = fopen(fileName, "w+");
    if (ob == NULL)
    {
        printf("failed to create .ob compiled image file for the source expanded source code\n");
        return;
    }
    writeMemoryImageToObFile(ob);
    fclose(ob);
    free(fileName);
}

void createEntriesFile(char *baseFileName)
{
    FILE *ent;
    char *fileName = calloc(strlen(baseFileName) + 3, sizeof(char *));
    sscanf(baseFileName, "%s", fileName);
    strcat(fileName, ".ent");
    ent = fopen(fileName, "w+");
    if (ent == NULL)
    {
        printf("failed to create .ent compiled file\n");
        return;
    }
    writeEntriesToFile(ent);
    fclose(ent);
    free(fileName);
}

/* void *initExternalsFile()
{
    FILE *ext;
    char *(*file)() = &getCurrentFileName;
    char *p, *fileName = calloc(strlen((*file)()) + 3, sizeof(char *));
    strcat(fileName, (*file)());
    p = strstr(fileName, "am");
    if (p)
    {
        p++;
        p = '\0';
        p--;
        p = '\0';
    }

    strcat(fileName, ".ext");
    ext = fopen(fileName, "w+");
    if (ext == NULL)
    {
        printf("failed to create .ext compiled file\n");
        return;
    }
    return ext;
}
 */