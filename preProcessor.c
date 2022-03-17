

#include "data.h"
extern State globalState;
extern Bool isLabelDeclaration(char *s);
extern Bool isOperation(char *s);
extern Bool isComment(char *s);
extern Bool isInstruction(char *s);
extern Bool verifyLabelNaming(char *s);
extern Item *addMacro(char *name, int start, int end, int linesLen);
extern Item *updateMacro(char *name, int start, int end, int linesLen);
extern void printMacroTable();
extern int printMacroItem(Item *item);

extern unsigned currentLine;
Bool isMacroOpening(char *s)
{
    printf("in is macro opening s:%s\n", s);

    return !strcmp(s, "macro") ? True : False;
}

Bool isMacroClosing(char *s)
{
    printf("in is macro closing, s:%s\n", s);
    return !strcmp(s, "endm") ? True : False;
}

Bool isLegalMacroName(char *s)
{
    return (strcmp(s, DATA) || strcmp(s, STRING) || strcmp(s, EXTERNAL) || strcmp(s, ENTRY)) && !isOperation(s) ? True : False;
}

void parseSourceFile(FILE *source, char *filename)
{

    FILE *newFile = createCopyFromSourceFile(source, filename);
    /* parseAll(newFile); */
    fclose(newFile);
}

int parseAll(FILE *newFile)
{
    ParseState state = newLine;
    int c = 0, i = 0, j = 0;
    int start = 1, end = 1, current = 1;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(newFile)) != EOF)
    {
        if (!isspace(c))
        {
            token[j++] = c;
        }
        else
        {
            if (j > 0)
            {
                state = evalToken;
            }
            if (i > 0 && state == parseMacroName)
            {
                state = parseMacroContent;
            }
        }
        if (c == '\n')
        {
            current++;
            currentLine++;
            memset(token, 0, j);
            j = 0;
            state = newLine;
        }
        /*
              macro m2
                inc r2
                sub r2,r6
                sub r1,r9
              endm

         */

        switch (state)
        {
        case skipLine:
        {

            memset(token, 0, MAX_LABEL_LEN);
            j = 0;
            memset(macroName, 0, MAX_LABEL_LEN);
            i = 0;
            state = newLine;
            break;
        }

        case evalToken:
        {
            printf("token:%s\n", token);
            if (isMacroOpening(token))
            {
                state = parseMacroName;
                start = current;
            }
            else if (isMacroClosing(token))
            {
                state = parseMacroContent;
            }
            else
            {
                memset(token, 0, MAX_LABEL_LEN);
                j = 0;
                state = skipLine;
            }
            token[j++] = ' ';
            break;
        }

        case newLine:
        {

            /*             if (isspace(c) && j > 0)
                        {
                            if (isMacroOpening(token))
                            {
                                state = parseMacroName;
                                start = current;
                            }
                            else if (isMacroClosing(token))
                            {
                                state = parseMacroContent;
                            }
                            else
                            {

                                memset(token, 0, MAX_LABEL_LEN);
                                j = 0;
                            }
                        } */

            break;
        }
        case parseMacroName:
        {
            printf("line 143, macroName:%s token:%s\n", macroName, token);

            if (!isspace(c))
                macroName[i++] = c;
            else
            {

                if (!isLegalMacroName(macroName))
                {
                    printf("before error, token:%s macroName:%s\n", token, macroName);
                    yieldError(illegalMacroNameUseOfSavedKeywords);

                    state = skipLine;
                }
                else
                {
                    state = parseMacroContent;
                    addMacro(macroName, start, -1, -1);
                }

                memset(token, 0, MAX_LABEL_LEN);
                j = 0;
            }

            break;
        }

        case parseMacroContent:
        {
            printf("line 172,token:%s macroName:%s\n", token, macroName);

            if (isMacroClosing(token))
            {

                end = currentLine;
                updateMacro(macroName, start, currentLine, currentLine - start);
                memset(macroName, 0, i + 1);
                i = 0;
                memset(token, 0, MAX_LABEL_LEN);
                j = 0;
                start = end = current;
            }
            else if (isMacroOpening(token))
            {

                memset(macroName, 0, i + 1);
                i = 0;
                memset(token, 0, MAX_LABEL_LEN);
                j = 0;
                start = end = current;
                state = skipLine;
                yieldError(useOfNestedMacrosIsIllegal);
            }
        }

        default:
            break;
        }
    }
    rewind(newFile);
    printMacroTable();

    return 1;
}

int parseNextLine(int start, int end)
{

    /*     printf("start:%d\nend:%d\n", start, end); */
    return 1;
}

int readFromFileByIndexes(FILE *fptr, char *filename, int start, int end)
{
    int c;
    if (fseek(fptr, end - start, start) != 0 || (c = fgetc(fptr)) == EOF)
    {
        /* yield some error abou


         */
    }

    return 1;
}
void replaceWithMacro(FILE *p, int startIndex, int endIndex)
{
}

void parseMacro(FILE *fp)
{
}

FILE *createCopyFromSourceFile(FILE *source, char *fileName)
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