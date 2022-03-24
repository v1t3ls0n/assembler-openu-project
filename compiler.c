#include "data.h"
/* Shared global State variables*/

extern int firstRunParsing(FILE *fp, char *filename);
extern Bool parseSingleLine(char *line, ...);
void createExpandedSourceFile(FILE *source, FILE *target, char *fileName);

extern void initTablesArrays();
extern void printBinaryImg();
extern void initMemory();
extern void resetMemory();
extern void updateFinalCountersValue();
extern void printMemoryImgInRequiredObjFileFormat();
extern void parseAssemblyCode(FILE *src, ...);
extern void exportFilesMainHandler(char *baseFileName);
extern void initExternalOperandsList();

extern State getGlobalState();
extern void updateGlobalState(State new);
extern void setFileNamePath(char *s);

extern void initAssemblyCodeFiles(char *fileName, assemblyCode *userCode);
void copyToNewFile(FILE *source, FILE *target);

Bool handleSingleSourceFile(char *arg);

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

Bool handleSingleSourceFile(char *arg)
{
    State (*globalState)() = &getGlobalState;
    FILE *src, *target;
    char *fileName = calloc(strlen(arg) + 3, sizeof(char *));
    /*     void (*setFileName)(char *) = &setCurrentFileName;
     */

    void (*setPath)(char *) = &setFileNamePath;

    (*setPath)(arg);
    strcpy(fileName, arg);

    /*     if (strstr(arg, "./"))
        {
            char *s;
            s = strrchr(arg, '/');
            s++;
            (*setFileName)(arg);
            s = strstr(arg, "./");
            (*setPath)(s);
        }
        else
        {
        }
     */
    if ((src = fopen(strcat(cloneString(fileName), ".as"), "r")) == NULL)
        return yieldError(fileCouldNotBeOpened);

    else if ((target = fopen(strcat(cloneString(fileName), ".am"), "w")) == NULL)
        return yieldError(fileCouldNotBeOpened);

    else
    {
        initTablesArrays();
        parseAssemblyCode(src, target);
        fclose(src);
        printMacroTable();
        if ((*globalState)() == firstRun)
        {
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

                parseAssemblyCode(src);
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

    /* free(fptrs); */

    fclose(target);
    free(fileName);
    return True;
}

void copyToNewFile(FILE *source, FILE *target)
{
    int c;
    while ((c = fgetc(source)) != EOF)
    {
        fputc(c, target);
    }
}