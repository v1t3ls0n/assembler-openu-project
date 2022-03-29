#include "data.h"

extern void fileCreationFailure(char *fileName);
static char *(*baseFileName)() = &getFileNamePath;

/*  void exportFilesMainHandler()
This function calls other functions that will create the .ob file,
.ent file and .ext file if there are any entry labels, and .ext file
if there are any external labels.*/

void exportFilesMainHandler()
{
    printf("Finished Successfully, about to export files!\n");
    generateObFile();
    if (areEntriesExist())
        createEntriesFile();
    if (areExternalsExist())
        createExternalsFile();
}

/* void generateObFile()
This function generates the .ob files by writing the memory image,
if the file can't be generate it yields error  */
void generateObFile()
{
    FILE *ob;
    char *fileName = (*baseFileName)();
    strcat(fileName, ".ob");
    ob = fopen(fileName, "w+");

    if (ob != NULL)
    {
        writeMemoryImageToObFile(ob);
        fclose(ob);
        free(fileName);
    }
    else
        fileCreationFailure(fileName);
}

/* void createEntriesFile()
This function generates the .ent files (as long as there are entry labels) by writing
it into the file, if the file can't be generate it yields error */
void createEntriesFile()
{
    FILE *ent;
    char *fileName = (*baseFileName)();
    strcat(fileName, ".ent");
    ent = fopen(fileName, "w+");

    if (ent != NULL)
    {
        writeEntriesToFile(ent);
        fclose(ent);
        free(fileName);
    }
    else
        fileCreationFailure(fileName);
}

/* createExternalsFile()
This function generates the .ext files (as long as there are external labels)
by writing it into the file, if the file can't be generate it yields error */
void createExternalsFile()
{
    FILE *ext;
    char *fileName = (*baseFileName)();
    strcat(fileName, ".ext");
    ext = fopen(fileName, "w+");
    if (ext != NULL)
    {
        writeExternalsToFile(ext);
        fclose(ext);
        free(fileName);
    }
    else
        fileCreationFailure(fileName);
}