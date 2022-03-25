#include "data.h"
/* Shared global State variables*/

extern int firstRunParsing(FILE *fp, char *filename);
extern Bool handleSingleLine(char *line);
void createExpandedSourceFile(FILE *source, FILE *target, char *fileName);

extern void initTablesArrays();
extern void printBinaryImg();
extern void initMemory();
extern void resetMemory();
extern void updateFinalCountersValue();
extern void printMemoryImgInRequiredObjFileFormat();
extern void parseAssemblyCode(FILE *src);
extern void exportFilesMainHandler();
extern void initExternalOperandsList();

extern void updateGlobalState(State new);

extern void initAssemblyCodeFiles(char *fileName, assemblyCode *userCode);
extern void parseSourceFile(FILE *src, FILE *target);
extern void closeOpenLogFiles();

void copyToNewFile(FILE *source, FILE *target);

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

    return True;
}

void handleSingleFile(char *arg)
{
    FILE *src, *target;
    void (*setPath)(char *) = &setFileNamePath;
    State (*globalState)() = &getGlobalState;
    char *fileName = (char *)calloc(strlen(arg), sizeof(char));
    strcpy(fileName, arg);
    strcat(fileName, ".as");
    if ((src = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "\n######################################################################\n");
        fprintf(stderr, " FAILURE! source code file %s could not be opened\n", arg);
        fprintf(stderr, "######################################################################\n\n");

        return;
    }

    fileName[strlen(fileName) - 1] = 'm';
    (*setPath)(fileName);

    if ((target = fopen(fileName, "w+")) == NULL)
    {

        fprintf(stderr, "\n######################################################################\n");
        fprintf(stderr, " FAILURE! expanded source code file %s could not be created\n", arg);
        fprintf(stderr, "######################################################################\n\n");

        return;
    }

    else
    {
        (*globalState)(parsingMacros);
        initTablesArrays();
        parseSourceFile(src, target);

        if ((*globalState)() == firstRun)
        {
            printMacroTable();
            rewind(target);
            parseAssemblyCode(target);
            if ((*globalState)() == secondRun)
            {
                rewind(target);
                updateFinalCountersValue();
                printSymbolTable();
                initMemory();
                if (areExternalsExist())
                    initExternalOperandsList();

                parseAssemblyCode(target);
                if ((*globalState)() == exportFiles)
                {
                    fileName[strlen(fileName) - 3] = '\0';
                    (*setPath)(fileName);
                    exportFilesMainHandler(fileName);
                    resetMemory();
                }
                else
                    printf("\nSecond Run Finished With Errors, files will not be exported!\n");
            }
        }
        else
            printf("\nfailed to create new .am (expanded source code) file for the %s source file\nmoving on to the next file if exist\n\n", arg);

        fclose(src);
        fclose(target);
        closeOpenLogFiles();
        free(fileName);
    }
}
