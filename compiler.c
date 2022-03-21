#include "data.h"
/* Shared global State variables*/

extern int firstRunParsing(FILE *fp, char *filename);
extern Bool parseSingleLine(char *line, State globalState);
void createExpandedSourceFile(FILE *source, FILE *target, char *fileName);

extern void initTablesArrays();
extern void printBinaryImg();
extern unsigned currentLineNumber;
extern void initMemory();
extern void updateFinalCountersValue();
extern void printMemoryImgInRequiredObjFileFormat();
extern void parseAssemblyCode(FILE *fp, char *filename, State globalState);
extern unsigned currentLineNumber;
extern State getGlobalState();
extern void updateGlobalState(State new);
void handleSingleSourceFile(char *arg);

int main(int argc, char *argv[])
{
    handleSourceFiles(argc, argv);

    return 0;
}

int handleSourceFiles(int argc, char *argv[])
{

    int filesCount = argc - 1;
    int i = 1;
    if (filesCount < 1)
    {
        yieldError(AssemblerDidNotGetSourceFiles);
        exit(1);
    }

    while (--argc)
    {
        updateGlobalState(parsingMacros);
        handleSingleSourceFile(argv[i]);
    }

    return True;
}

void handleSingleSourceFile(char *arg)
{
    State (*globalState)() = &getGlobalState;
    FILE *fptr, *expandedSrc;
    char *fileName = calloc(strlen(arg) + 3, sizeof(char *));
    sscanf(arg, "%s", fileName);
    if ((fptr = fopen(strcat(fileName, ".as"), "r")) == NULL)
    {
        yieldError(fileCouldNotBeOpened);
        return;
    }
    initTablesArrays();
    expandedSrc = fopen(strcat(fileName, ".am"), "w+");
    if (expandedSrc == NULL)
    {
        fclose(fptr);
        updateGlobalState(goToNextFileOrEndProgram);
        return NULL;
    }
    createExpandedSourceFile(fptr, expandedSrc, fileName);
    if ((*globalState)() == firstRun)
    {
        printMacroTable();
        initMemory();
        parseAssemblyCode(expandedSrc, fileName, firstRun);
        if ((*globalState)() == secondRun)
        {
            rewind(fptr);
            updateFinalCountersValue();
            printSymbolTable();
            parseAssemblyCode(expandedSrc, fileName, secondRun);
            if ((*globalState)() == exportFiles)
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
    else
        printf("failed to create new .am (expanded source code) file for the %s source file\nmoving on to the next file if exist", fileName);

    free(fileName);
}
