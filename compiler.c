#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern void parseSourceFile(FILE *source, char *filename);
extern int firstRunParsing(FILE *fp, char *filename);
extern Bool parseSingleLine(char *line);

extern void initTablesArrays();
extern void printBinaryImg();
extern unsigned currentLineNumber;
extern void initMemory();
extern void updateFinalCountersValue();
extern void printMemoryImgInRequiredObjFileFormat();
extern void parseAssemblyCode(FILE *fp, char *filename);
extern unsigned currentLineNumber;

int main(int argc, char *argv[])
{
    initTablesArrays();
    globalState = parsingMacros;
    handleSourceFiles(argc, argv);
    globalState = firstRun;
    handleSourceFiles(argc, argv);
    if (globalState != collectErrors)
    {
        globalState = secondRun;
        updateFinalCountersValue();
        printSymbolTable();
        initMemory();
        handleSourceFiles(argc, argv);
    }
    else
        printf("\nFinished First Run With Errors\n");

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
        fileName = globalState == parsingMacros ? strcat(fileName, ".as") : strcat(fileName, ".am");

        if ((fptr = fopen(fileName, "r")) == NULL)
            yieldError(fileCouldNotBeOpened);
        else
        {
            if (globalState == parsingMacros)
                parseSourceFile(fptr, fileName);
            else if (globalState == firstRun)
                parseAssemblyCode(fptr, fileName);

            else if (globalState == secondRun)
            {
                rewind(fptr);
                parseAssemblyCode(fptr, fileName);
                if (globalState != collectErrors)
                {
                    printf("Finished Successfully, about to export files!\n");
                    printBinaryImg();
                    printf("\n");
                    printMemoryImgInRequiredObjFileFormat();
                }
                else
                    printf("\nSecond Run Finished With Errors, files will not be exported!\n");
            }
        }

        free(fileName);
    }

    return True;
}
