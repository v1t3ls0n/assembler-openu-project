#include "data.h"

int main(int argc, char *argv[])
{
    /*
    we are passing the command line arguments that are the files that passed to the assembler
    to the handleSourceFiles function.
     */
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
    closeOpenLogFiles();

    return 0;
}
void handleSingleFile(char *arg)
{
    FILE *src = NULL, *target = NULL;
    void (*setPath)(char *) = &setFileNamePath;
    State (*globalState)() = &getGlobalState;
    void (*setState)(State) = &setGlobalState;

    char *fileName = (char *)calloc(strlen(arg) + 4, sizeof(char *));
    extern void resetMemoryCounters();
    extern void initTables();
    extern void exportFilesMainHandler();
    extern void closeOpenLogFiles();
    extern void allocMemoryImg();
    extern void calcFinalAddrsCountersValues();
    extern void freeHashTable(ItemType type);

    strncpy(fileName, arg, strlen(arg)); /* sets the fileName to be arg */
    strcat(fileName, ".as");             /* turns the files into .as files */
    (*setPath)(fileName);
    /* if the file couldn't be opened it alerts it to the user */
    if ((src = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "\n######################################################################\n");
        fprintf(stderr, " FAILURE! source code file %s could not be opened\n", fileName);
        fprintf(stderr, "######################################################################\n\n");
        free(fileName);
        return;
    }

    fileName[strlen(fileName) - 1] = 'm'; /* change the files to be .am files */
    (*setPath)(fileName);
    /* if the expanded soure file couldn't be created it alerts it to the user */
    if ((target = fopen(fileName, "w+")) == NULL)
    {

        fprintf(stderr, "\n######################################################################\n");
        fprintf(stderr, " FAILURE! expanded source code file %s could not be created\n", fileName);
        fprintf(stderr, "######################################################################\n\n");
        fclose(src);
        free(fileName);
        return;
    }

    else
    {
        initTables();

        /* if there are no errors it starts the pre proccesing- parse all the macros,
         saves them in the macro table and prints it */
        (*setState)(parsingMacros);
        resetMemoryCounters();
        parseSourceFile(src, target);
        printMacroTable();
        freeHashTable(Macro);

        /* moves on to the first run, looks for errors in the code, counts how much space in the
         memory the program needs and starts to parse the assembly code.
         */
        if ((*globalState)() == firstRun)
        {

            rewind(target);
            parseAssemblyCode(target);
            /* if the first run ended with no errors, it moves on to the second run,
            builds the memory image, prints the symbols table */
            if ((*globalState)() == secondRun)
            {
                calcFinalAddrsCountersValues();
                updateFinalSymbolTableValues();
                allocMemoryImg();
                printSymbolTable();
                rewind(target);
                parseAssemblyCode(target);
                /* if the second run ended with no errors, it moves to export file
                and creates the additional files (.ob, .ent and .ext files) */
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
            printf("\nexpanding macros for %s source file\nfailed due to some code errors\nmoving on to the next file if exist\n\n", fileName);

        /* frees and closes all the files that have been in use */
        free(fileName);
        fclose(src);
        fclose(target);
    }
}