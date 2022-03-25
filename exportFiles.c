#include "data.h"

/* void generateObFile();
void createEntriesFile();
extern char *getFileNamePath(); */
char *(*baseFileName)() = &getFileNamePath;

void exportFilesMainHandler()
{
    printf("Finished Successfully, about to export files!\n");
    generateObFile();
    if (areEntriesExist())
        createEntriesFile();
    if (areExternalsExist())
        createExternalsFile();
}

void generateObFile()
{
    FILE *ob;
    char *fileName = (*baseFileName)();
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

void createEntriesFile()
{
    FILE *ent;
    char *fileName = (*baseFileName)();
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

void createExternalsFile()
{
    FILE *ext;
    char *fileName = (*baseFileName)();
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