#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern void parseSourceFile(FILE *fp, char *filename);
extern void parseExpandedSourceFile(FILE *fp, char *filename);

int main(int argc, char *argv[])
{
    FILE *fptr;
    char fileName[30] = {0};
    int filesCount = argc - 1;
    int i = 1;

    if (filesCount < 1)
    {
        yieldError(AssemblerDidNotGetSourceFiles);
        exit(1);
    }

    while (--filesCount)
    {
        sscanf(argv[i], "%s", fileName);
        if ((fptr = fopen(strcat(argv[i], ".as"), "r")) == NULL)
            yieldError(fileCouldNotBeOpened);
        else
        {
            if (globalState == handleMacros)
                parseSourceFile(fptr, argv[i]);

            if (globalState == firstRun)
                parseExpandedSourceFile(fptr, argv[i]);

            fclose(fptr);
        }
    }

    printf("Finished Successfully!\n");
    return 0;
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
