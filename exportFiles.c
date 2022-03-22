#include "data.h"

extern void writeMemoryImageToObFile(FILE *fp);
extern void writeEntriesToFile(FILE *fp);
extern Bool areEntriesExist();
extern Bool areExternalsExist();
extern void writeExternalsToFile(FILE *fp);

void generateObFile(char *baseFileName);
void createEntriesFile(char *baseFileName);

void exportFilesMainHandler(char *baseFileName)
{
    generateObFile(baseFileName);
    if (areEntriesExist())
        createEntriesFile(baseFileName);
    if (areExternalsExist())
    {
    }
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
        printf("failed to create .ent file\n");
        return;
    }
    writeEntriesToFile(ent);
    fclose(ent);
    free(fileName);
}

void createEnternalsFile(char *baseFileName)
{
    FILE *ext;
    char *fileName = calloc(strlen(baseFileName) + 3, sizeof(char *));
    sscanf(baseFileName, "%s", fileName);
    strcat(fileName, ".ext");
    ext = fopen(fileName, "w+");
    if (ext == NULL)
    {
        printf("failed to create .ext file\n");
        return;
    }
    writeExternalsToFile(ext);
    fclose(ext);
    free(fileName);
}