

#include "data.h"
extern State globalState;
extern Bool isLabel(char* s);
extern Bool isOperation(char* s);
extern Bool isComment(char* s);
extern Bool isInstruction(char* s);
extern Bool verifyLabelNaming(char* s);
extern Item* addMacro(char* name, int start, int end, int linesLen);
extern Item* updateMacro(char* name, int start, int end, int linesLen);
extern void printMacroTable();
extern int printMacroItem(Item* item);
Bool isMacroOpening(char* s)
{
    return !strcmp(s, "macro") ? True : False;
}

Bool isMacroClosing(char* s)
{
    printf("in is macro closing, s:%s\n", s);

    return !strcmp(s, "endm") ? True : False;
}

Bool isLegalMacroName(char* s)
{

    return !isInstruction(s) && !isOperation(s);
}

void parseSourceFile(FILE* source, char* filename)
{

    int c = 0, i = 0, j = 0;
    int start = 1, end = 1, current = 1;

    char token[MAX_LINE_LEN] = { 0 };
    char macroName[MAX_LABEL_LEN] = { 0 };
    ParseState state = newLine;
    FILE* newFile = createCopyFromSourceFile(source, filename);
    rewind(newFile);
    while ((c = fgetc(newFile)) != EOF)
    {
        if (c == '\n')
        {
            current++;
            memset(token, 0, j);
            j = 0;
        }

        if (!isspace(c) && isprint(c))
            token[j++] = c;

        switch (state)
        {
        case skipLine:
        {

            memset(token, 0, MAX_LABEL_LEN);
            memset(macroName, 0, MAX_LABEL_LEN);
            j = 0;
            i = 0;
            state = newLine;
            break;
        }

        case newLine:
        {
            /*             printf("in new line state\n");
             */
            if (isspace(c) && j > 0)
            {
                if (isMacroOpening(token))
                {
                    state = parseMacroName;
                    start = current;
                }
            }

            break;
        }
        case parseMacroName:
        {

            if (!isspace(c) && isprint(c))
            {
                macroName[i++] = c;
                /*         printf("inside  if (!isspace(c) && isprint(c))\n"); */
            }
            /*             printf("in macro name state\nmacroName:%s i:%d\n", macroName, i);
             */
            if (isspace(c) && i > 0)
            {
                /*     printf("token:%s macroName:%s\n", token, macroName); */
                if (isLegalMacroName(macroName))
                {
                    state = parseMacroContent;
                    addMacro(macroName, start, current, current - start);
                }
                else
                {
                    yieldError(illegalMacroNameUseOfSavedKeywords);
                    memset(macroName, 0, i + 1);
                    i = 0;
                }
            }

            break;
        }

        case parseMacroContent:
        {

            if (!isspace(c))
            {
                if (isMacroClosing(token) || isMacroOpening(token))
                {
                    end = current;
                    updateMacro(macroName, start, end, end - start);
                    state = skipLine;
                }
            }
        }

        default:
            break;
        }
    }
    printMacroTable();
    fclose(newFile);
}

int parseNextLine(int start, int end)
{

    /*     printf("start:%d\nend:%d\n", start, end); */
    return 1;
}

int readFromFileByIndexes(FILE* fptr, char* filename, int start, int end)
{
    int c;
    if (fseek(fptr, end - start, start) != 0 || (c = fgetc(fptr)) == EOF)
    {
        /* yield some error abou


         */
    }

    return 1;
}
void replaceWithMacro(FILE* p, int startIndex, int endIndex)
{
}

void parseMacro(FILE* fp)
{
}

FILE* createCopyFromSourceFile(FILE* source, char* fileName)
{
    FILE* target;
    int c = 0;
    fileName[strlen(fileName) - 1] = 'm';
    target = fopen(fileName, "w+");
    if (target == NULL)
    {
        fclose(source);
        printf("Press any key to exit...\n");
        exit(1);
    }
    while ((c = fgetc(source)) != EOF)
    {
        fputc(c, target);
    }

    printf("File copied successfully.\n");
    fclose(source);
    /*     fclose(target); */

    return target;
}
/*
FILE *createExpendedSourceFileWithoutMacros(FILE *source, char *fileName)
{
    FILE *target;
    int c = 0;
    fileName[strlen(fileName) - 1] = 'm';
    target = fopen(fileName, "w+");
    if (target == NULL)
    {
        fclose(source);
        printf("Press any key to exit...\n");
        exit(1);
    }
    while ((c = fgetc(source)) != EOF)
    {

        fputc(c, target);
    }

    printf("File copied successfully.\n");
    fclose(source);
    fclose(target);

    return target;
} */