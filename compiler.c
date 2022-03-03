#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern void parseSourceFile(FILE *source, char *filename);
extern int parseExpandedSourceFile(FILE *fp, char *filename);

int main(int argc, char *argv[])
{

    handleSourceFiles(argc, argv);
    globalState = firstRun;
    printf("after macro state\n");
    handleSourceFiles(argc, argv);

    if (globalState != collectErrors)
        printf("Finished Successfully!\n");
    else
        printf("\nFinished First Run With Errors\n");

    printSymbolTable();

    /*
    symbol table from test should include:
    x data entry
    y data
    Bla external
    dsafs external
    str2 string
    str string
    */

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
        printf("fileName:%s\n", fileName);
        fileName = globalState == handleMacros ? strcat(fileName, ".as") : strcat(fileName, ".am");

        if ((fptr = fopen(fileName, "r")) == NULL)
            yieldError(fileCouldNotBeOpened);
        else
        {
            if (globalState == handleMacros)
                parseSourceFile(fptr, fileName);
            if (globalState == firstRun)
                parseExpandedSourceFile(fptr, fileName);

            fclose(fptr);
            free(fileName);
        }
    }

    return True;
}

/*
    parseSingleLine(".entry      x");
    parseSingleLine("x:            .data          3        ");
    parseSingleLine("x:            .string          3        ");
    parseSingleLine("y:            .data          5, 8,     56        ");
    parseSingleLine("d:            .data          5, $,     56        ");
    parseSingleLine("z:            .data          7      ,  ,   56,        ");
    parseSingleLine("fg:            .data          7      ,  ,   44,$^        ");
    parseSingleLine("g:            .data          7      ,  ,   ,,,56,        ");
    parseSingleLine(".external Bla");
    parseSingleLine(".external stamm      ddd   ");
    parseSingleLine(".external dsafs   ");
    parseSingleLine("str:            .string          \"abcd\"        ");
    parseSingleLine("str2:            .string          \"AAADDDDDabcd\"        ");
    parseSingleLine("str3:            .string          \"FFFFFFFFAAADDDDDabcd\"        ");

        printSymbolTable();


*/
