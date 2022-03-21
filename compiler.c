#include "data.h"
/* Shared global State variables*/

extern int firstRunParsing(FILE *fp, char *filename);
extern Bool parseSingleLine(char *line);
extern State createExpandedSourceFile(FILE *source, char *fileName);

extern void initTablesArrays();
extern void printBinaryImg();
extern unsigned currentLineNumber;
extern void initMemory();
extern void updateFinalCountersValue();
extern void printMemoryImgInRequiredObjFileFormat();
extern State parseAssemblyCode(FILE *fp, char *filename, State globalState);
extern unsigned currentLineNumber;

State globalState = startProgram;

int main(int argc, char *argv[])
{

    handleSourceFiles(argc, argv);

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
            initTablesArrays();
            globalState = createExpandedSourceFile(fptr, fileName);
            if (globalState == firstRun)
            {
                printMacroTable();
                initMemory();
                globalState = parseAssemblyCode(fptr, fileName, firstRun);
                if (globalState == secondRun)
                {
                    rewind(fptr);
                    updateFinalCountersValue();
                    printSymbolTable();
                    parseAssemblyCode(fptr, fileName, secondRun);
                    if (globalState == exportFiles)
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
        }

        free(fileName);
    }

    return True;
}
