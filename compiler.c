#include "data.h"
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
        fprintf(stderr, "\n\nYou did not passed any source files to the assembler!\n\n");
        exit(1);
    }
    while (--argc)
    {
        handleSingleFile(argv[i]);
        i++;
    }
    return 0;
}

void handleSingleFile(char *arg)
{
    FILE *src = NULL, *target = NULL;
    void (*setPath)(char *) = &setFileNamePath;
    State (*globalState)() = &getGlobalState;
    char *fileName = (char *)calloc(strlen(arg) + 4, sizeof(char *));
    extern void resetMemoryCounters();
    extern void initTables();
    extern void exportFilesMainHandler();
    extern void closeOpenLogFiles();
    extern void allocMemoryImg();
    extern void calcFinalAddrsCountersValues();
    extern void freeHashTable(ItemType type);

    strncpy(fileName, arg, strlen(arg));
    strcat(fileName, ".as");
    (*setPath)(fileName);

    if ((src = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "\n######################################################################\n");
        fprintf(stderr, " FAILURE! source code file %s could not be opened\n", fileName);
        fprintf(stderr, "######################################################################\n\n");
        /*         free(fileName);
                return; */
    }

    fileName[strlen(fileName) - 1] = 'm';
    (*setPath)(fileName);
    if ((target = fopen(fileName, "w+")) == NULL)
    {

        fprintf(stderr, "\n######################################################################\n");
        fprintf(stderr, " FAILURE! expanded source code file %s could not be created\n", fileName);
        fprintf(stderr, "######################################################################\n\n");
        fclose(src);
        /*         free(fileName);
                return; */
    }

    else
    {
        (*globalState)(parsingMacros);
        initTables();
        resetMemoryCounters();
        parseSourceFile(src, target);
        printMacroTable();
        freeHashTable(Macro);

        if ((*globalState)() == firstRun)
        {

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
            else
                printf("\nFirst Run Finished With Errors, will no enter second run and files will not be exported!\n");

            freeHashTable(Symbol);
        }
        else
            printf("\nfailed to create new .am (expanded source code) file for the %s source file\nmoving on to the next file if exist\n\n", fileName);

        free(fileName);
        fclose(src);
        fclose(target);
        closeOpenLogFiles();
    }
}