#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern void parseSourceFile(FILE* source, char* filename);
extern int firstRunParsing(FILE* fp, char* filename);
extern void initTablesArrays();
extern void printBinaryImg();
extern unsigned currentLine;
extern void initMemory();

int main(int argc, char* argv[])
{
    globalState = replacingMacros;
    handleSourceFiles(argc, argv);
    initTablesArrays();
    globalState = firstRun;
    handleSourceFiles(argc, argv);
    if (globalState != collectErrors) {
        printf("global state is not collect errors after first run\n");
        globalState = secondRun;
        handleSourceFiles(argc, argv);
    }
    else
        printf("\nFinished First Run With Errors\n");

    if (globalState != collectErrors)
    {
        printf("Finished Successfully, about to export files!\n");
        printSymbolTable();
        printBinaryImg();
    }
    else
        printf("\nSecond Run Finished With Errors, files will not be exported!\n");



    return 0;
}

int handleSourceFiles(int argc, char* argv[])
{
    FILE* fptr;
    char* fileName;
    int filesCount = argc - 1;
    int i = 1;
    if (filesCount < 1)
    {
        yieldError(AssemblerDidNotGetSourceFiles);
        exit(1);
    }

    while (--argc)
    {
        fileName = calloc(strlen(argv[i]) + 3, sizeof(char*));
        sscanf(argv[i], "%s", fileName);
        fileName = globalState == replacingMacros ? strcat(fileName, ".as") : strcat(fileName, ".am");

        if ((fptr = fopen(fileName, "r")) == NULL)
            yieldError(fileCouldNotBeOpened);
        else
        {
            if (globalState == replacingMacros)
                parseSourceFile(fptr, fileName);
            else  if (globalState == firstRun)
            {
                firstRunParsing(fptr, fileName);
                rewind(fptr);

            }
            else if (globalState == secondRun) {
                secondRunParsing(fptr, fileName);

            }



        }
    }

    free(fileName);


    return True;
}
