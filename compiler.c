#include "data.h"

/* extern int firstRunParsing(FILE *fp, char *filename);
extern Bool handleSingleLine(char *line);
void createExpandedSourceFile(FILE *source, FILE *target, char *fileName);
extern void printBinaryImg();
extern void allocMemoryImg();

extern void calcFinalAddrsCountersValues();
extern void printMemoryImgInRequiredObjFileFormat();
extern void parseAssemblyCode(FILE *src);
extern void exportFilesMainHandler();
extern void initExternalOperandsList();
extern void setGlobalState(State new);
extern void parseSourceFile(FILE *src, FILE *target);
extern void closeOpenLogFiles(); */
extern void resetMemoryCounters();
extern void initTables();
extern void exportFilesMainHandler();
extern void closeOpenLogFiles();
extern void allocMemoryImg();
extern void calcFinalAddrsCountersValues();
extern void fileOpeningFailure(char *fileName);

void handleSingleFile(char *arg);

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
        handleSingleFile(argv[i]);
        i++;
    }
    closeOpenLogFiles();
    return True;
}

void handleSingleFile(char *arg)
{
    FILE *src = NULL, *target = NULL;
    void (*setPath)(char *) = &setFileNamePath;
    State (*globalState)() = &getGlobalState;
    char *fileName = (char *)calloc(strlen(arg), sizeof(char *));
    if (!fileName)
    {
        yieldError(memoryAllocationFailure);
        return;
    }
    memcpy(fileName, arg, strlen(arg));
    strcat(fileName, ".as");
    (*setPath)(fileName);
    if ((src = fopen(fileName, "r")) == NULL)
    {
        fileOpeningFailure(fileName);
        free(fileName);
        return;
    }

    fileName[strlen(fileName) - 1] = 'm';
    (*setPath)(fileName);
    if ((target = fopen(fileName, "w+")) == NULL)
    {
        fileCreationFailure(fileName);
        fclose(src);
        free(fileName);
        return;
    }

    (*globalState)(parsingMacros);
    resetMemoryCounters();
    initTables();
    parseSourceFile(src, target);

    if ((*globalState)() == firstRun)
    {
        printMacroTable();
        rewind(target);
        parseAssemblyCode(target);

        if ((*globalState)() == secondRun)
        {
            calcFinalAddrsCountersValues();
            updateFinalSymbolTableValues();
            allocMemoryImg();
            printSymbolTable();
            rewind(target);
            parseAssemblyCode(target);
            if ((*globalState)() == exportFiles)
            {
                fileName[strlen(fileName) - 3] = '\0';
                (*setPath)(fileName);
                exportFilesMainHandler();
            }
            else
                printf("\nSecond Run Finished With Errors, files will not be exported!\n");
        }
    }
    else
        printf("\nfailed to create new .am (expanded source code) file for the %s source file\nmoving on to the next file if exist\n\n", arg);

    fclose(src);
    free(fileName);
    fclose(target);
}
