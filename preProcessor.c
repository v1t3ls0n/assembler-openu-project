

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

int parseAll(FILE *newFile);

Bool isMacroOpening(char *s)
{
    /*     printf("in is macro opening s:%s\n", s);
     */
    return !strcmp(s, "macro") ? True : False;
}

Bool isMacroClosing(char *s)
{
    /*     printf("in is macro closing, s:%s\n", s); */
    return !strcmp(s, "endm") ? True : False;
}

Bool isLegalMacroName(char *s)
{
    return (strcmp(s, DATA) && strcmp(s, STRING) && strcmp(s, EXTERNAL) && strcmp(s, ENTRY)) && !isOperation(s) ? True : False;
}

void parseSourceFile(FILE *source, char *filename)
{

    FILE *newFile = createCopyFromSourceFile(source, filename);
    rewind(newFile);
    parseAll(newFile);
    fclose(newFile);
    printMacroTable();
}

int parseAll(FILE *newFile)
{
    ParseState state = evalToken;
    Bool isMacro = False;
    int c = 0, i = 0, j = 0;
    int start = 1, end = 1, current = 1;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(newFile)) != EOF)
    {
        if (c == '\n' && state == skipLine)
        {
            printf("line 58 macroName:%s token:%s\n", macroName, token);
            state = evalToken;
        }

        if (state != skipLine)
        {

            if (!isspace(c) && j < MAX_LABEL_LEN && i < MAX_LABEL_LEN)
            {
                if (state == parsingMacroName)
                    macroName[i++] = c;

                else
                    token[j++] = c;
            }
            else if (isspace(c) && (j > 0 || i > 0))
            {

                if (i > 0)
                    macroName[i] = '\0';
                if (j > 0)
                    token[j] = '\0';

                printf("c:%c i:%d j:%d macroName:%s token:%s\n", c, i, j, macroName, token);

                if (state == evalToken)
                {
                    if (!isMacro && isMacroClosing(token))
                    {
                        yieldError(macroClosingWithoutAnyOpenedMacro);
                        state = skipLine;
                    }

                    else if (!isMacro && isMacroOpening(token))
                    {
                        state = parsingMacroName;
                        isMacro = True;
                    }
                    else if (isMacro)
                    {

                        if (isMacroClosing(token))
                        {
                            end = c == '\n' ? currentLine + 1 : currentLine;
                            updateMacro(macroName, start, end, end - start);
                            memset(macroName, 0, i);
                            i = 0;
                            isMacro = False;
                            state = skipLine;
                        }
                        else if (isMacroOpening(token))
                        {
                            yieldError(useOfNestedMacrosIsIllegal);
                            updateMacro(macroName, -1, -1, -1);
                            memset(macroName, 0, i);
                            i = 0;
                            isMacro = False;
                            state = skipLine;
                        }
                    }

                    memset(token, 0, j);
                    j = 0;
                }

                else if (state == parsingMacroName)
                {

                    if (isLegalMacroName(macroName))
                    {
                        start = current;
                        addMacro(macroName, start, -1, -1);
                        i = 0;
                        state = evalToken;
                    }
                    else
                    {
                        yieldError(illegalMacroNameUseOfSavedKeywords);
                        memset(macroName, 0, i);
                        i = 0;
                        state = skipLine;
                        isMacro = False;
                    }
                }
            }
        }
        if (c == '\n')
        {
            current++;
            currentLine++;
            /*             state = evalToken;
                        memset(token, 0, j);
                        j = 0; */
        }
    }

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
