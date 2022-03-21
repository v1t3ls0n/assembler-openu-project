#include "data.h"
/* Shared global State variables*/

extern int firstRunParsing(FILE *fp, char *filename);
extern Bool parseSingleLine(char *line, State globalState);
void createExpandedSourceFile(FILE *source, FILE *target, char *fileName);

extern void initTablesArrays();
extern void printBinaryImg();
extern void initMemory();
extern void resetMemory();
extern void updateFinalCountersValue();
extern void printMemoryImgInRequiredObjFileFormat();
extern void parseAssemblyCode(FILE *fp, char *filename, State globalState);

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
        i++;
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
    fileName[strlen(fileName) - 1] = 'm';
    expandedSrc = fopen(fileName, "w+");
    if (expandedSrc == NULL)
    {
        fclose(fptr);
        updateGlobalState(goToNextFileOrEndProgram);
    }
    else
    {
        createExpandedSourceFile(fptr, expandedSrc, fileName);
        rewind(expandedSrc);
        if ((*globalState)() == firstRun)
        {
            printMacroTable();
            parseAssemblyCode(expandedSrc, fileName, firstRun);
            if ((*globalState)() == secondRun)
            {
                rewind(expandedSrc);
                updateFinalCountersValue();
                printSymbolTable();
                initMemory();
                parseAssemblyCode(expandedSrc, fileName, secondRun);
                if ((*globalState)() == exportFiles)
                {
                    printf("Finished Successfully, about to export files!\n");
                    printBinaryImg();
                    printf("\n");
                    printMemoryImgInRequiredObjFileFormat();
                    resetMemory();
                }
                else
                    printf("\nSecond Run Finished With Errors, files will not be exported!\n");
            }
        }
        else
            printf("failed to create new .am (expanded source code) file for the %s source file\nmoving on to the next file if exist", fileName);
    }

    free(fileName);
    fclose(fptr);
    fclose(expandedSrc);
}
