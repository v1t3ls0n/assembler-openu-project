#include "data.h"
/* Shared global State variables*/

extern int firstRunParsing(FILE *fp, char *filename);
extern Bool parseSingleLine(char *line, assemblyCode *fptrs);
void createExpandedSourceFile(FILE *source, FILE *target, char *fileName);

extern void initTablesArrays();
extern void printBinaryImg();
extern void initMemory();
extern void resetMemory();
extern void updateFinalCountersValue();
extern void printMemoryImgInRequiredObjFileFormat();
extern void parseAssemblyCode(assemblyCode *fptrs);
extern void exportFilesMainHandler(char *baseFileName);
extern void initExternalOperandsList();

extern State getGlobalState();
extern void updateGlobalState(State new);
extern void setFileNamePath(char *s);

extern void initAssemblyCodeFiles(char *fileName, assemblyCode *userCode);
void copyToNewFile(FILE *source, FILE *target);

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
    assemblyCode *fptrs = (assemblyCode *)malloc(sizeof(assemblyCode));
    char *fileName = calloc(strlen(arg) + 3, sizeof(char *));
    void (*setFileName)(char *) = &setCurrentFileName;
    void (*setPath)(char *) = &setFileNamePath;
    if (strstr(arg, "./"))
    {
        char *s;
        s = strrchr(arg, '/');
        s++;
        sscanf(s, "%s", fileName);
        (*setFileName)(arg);
        s = strstr(arg, "./");
        (*setPath)(s);
    }
    else
    {
        (*setPath)(arg);
    }
    sscanf(arg, "%s", fileName);
    fptrs->src = (FILE *)malloc(sizeof(FILE *));
    fptrs->expanded = (FILE *)malloc(sizeof(FILE *));
    initAssemblyCodeFiles(arg, fptrs);
    if (!fptrs)
    {
        return;
    }
    else
    {

        initTablesArrays();
        copyToNewFile(fptrs->src, fptrs->expanded);
        rewind(fptrs->src);
        rewind(fptrs->expanded);
        parseAssemblyCode(fptrs);
        printMacroTable();

        if ((*globalState)() == firstRun)
        {
            /*    initTablesArrays(); */
            parseAssemblyCode(fptrs);
            if ((*globalState)() == secondRun)
            {
                rewind(fptrs->expanded);
                updateFinalCountersValue();
                printSymbolTable();
                initMemory();
                if (areExternalsExist())
                    initExternalOperandsList();
                parseAssemblyCode(fptrs);

                if ((*globalState)() == exportFiles)
                {
                    exportFilesMainHandler(fileName);
                    resetMemory();
                }
                else
                    printf("\nSecond Run Finished With Errors, files will not be exported!\n");
            }
        }
        else
            printf("failed to create new .am (expanded source code) file for the %s source file\nmoving on to the next file if exist", fileName);
    }

    /*     free(fileName); */
    fclose(fptrs->src);
    fclose(fptrs->expanded);
}

void copyToNewFile(FILE *source, FILE *target)
{
    int c;
    while ((c = fgetc(source)) != EOF)
    {
        fputc(c, target);
    }
}