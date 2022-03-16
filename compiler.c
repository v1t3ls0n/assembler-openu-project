#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern void parseSourceFile(FILE *source, char *filename);
extern int firstRunParsing(FILE *fp, char *filename);
extern void parseSingleLine(char *line);

extern void initTablesArrays();
extern void printBinaryImg();
extern unsigned currentLine;
extern void initMemory();
extern void updateFinalCountersValue();
extern void printMemoryImgInRequiredObjFileFormat();
extern Bool parseFile(FILE *fp, char *filename);
extern unsigned currentLine;

int main(int argc, char *argv[])
{
    initTablesArrays();
    globalState = replacingMacros;
    handleSourceFiles(argc, argv);
    globalState = firstRun;
    handleSourceFiles(argc, argv);

    if (globalState != collectErrors)
    {
        updateFinalCountersValue();
        printSymbolTable();
        initMemory();
        globalState = secondRun;
        handleSourceFiles(argc, argv);
    }
    else
        printf("\nFinished First Run With Errors\n");

    if (globalState != collectErrors)
    {
        printf("Finished Successfully, about to export files!\n");
        printBinaryImg();
        printf("\n");
        printMemoryImgInRequiredObjFileFormat();
    }
    else
        printf("\nSecond Run Finished With Errors, files will not be exported!\n");

    return 0;
}

int handleSourceFiles(int argc, char *argv[])
{
    FILE *fptr;
    char *fileName;
    int filesCount = argc - 1;
    int i = 1;
    if (filesCount < 1)
    {
        yieldError(AssemblerDidNotGetSourceFiles);
        exit(1);
    }

    while (--argc)
    {
        fileName = calloc(strlen(argv[i]) + 3, sizeof(char *));
        sscanf(argv[i], "%s", fileName);
        fileName = globalState == replacingMacros ? strcat(fileName, ".as") : strcat(fileName, ".am");

        if ((fptr = fopen(fileName, "r")) == NULL)
            yieldError(fileCouldNotBeOpened);
        else
        {
            if (globalState == replacingMacros)
                parseSourceFile(fptr, fileName);
            else if (globalState == firstRun)
            {
                parseFile(fptr, fileName);
            }
            else if (globalState == secondRun)
            {
                rewind(fptr);
                parseFile(fptr, fileName);
            }
        }

        free(fileName);
    }

    return True;
}
