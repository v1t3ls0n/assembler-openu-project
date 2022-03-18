

#include "data.h"
extern State globalState;
extern Bool isLabelDeclaration(char *s);
extern Bool isOperation(char *s);
extern Bool isComment(char *s);
extern Bool isInstructionStrict(char *s);
extern Bool verifyLabelNaming(char *s);
extern Item *addMacro(char *name, int start, int end);
extern Item *updateMacro(char *name, int start, int end);
extern Item *getMacro(char *s);

extern void printMacroTable();
extern int printMacroItem(Item *item);
extern unsigned currentLine;

extern Bool isPossiblyUseOfMacro(char *s);
extern Bool isMacroOpening(char *s);
extern Bool isMacroClosing(char *s);
extern Bool isLegalMacroName(char *s);

void saveMacros(FILE *target);
void popLastToken(FILE *target, char *token);
void parseAndReplaceMacros(FILE *source, FILE *target);
void replaceWithMacro(FILE *target, FILE *source, int start, int end);
FILE *createExpandedSourceFile(FILE *source, char *fileName);

void parseSourceFile(FILE *source, char *filename)
{

    FILE *target = createExpandedSourceFile(source, filename);
    rewind(target);
    fclose(target);
    printMacroTable();
}

void parseAndReplaceMacros(FILE *source, FILE *target)
{
    ParseState state = evalToken;
    Bool isMacroCurrentlyParsed = False;
    Bool isMacroStartFoundYet = False;
    int c = 0, i = 0, j = 0, current = 0;
    int start = 0, end = 0;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(source)) != EOF)
    {
        if (!isMacroCurrentlyParsed)
            fputc(c, target);

        if (isMacroCurrentlyParsed && isMacroStartFoundYet)
            end++;
        if (c == '\n')
        {
            currentLine++;
            if (state == skipLine)
                state = evalToken;
        }

        if (state != skipLine)
        {
            if (isMacroCurrentlyParsed && !isMacroStartFoundYet)
            {
                start = ftell(source) - 1;
                isMacroStartFoundYet = True;
            }

            if (!isspace(c))
            {

                if (j < MAX_LABEL_LEN && i < MAX_LABEL_LEN)
                {

                    if (state == parsingMacroName)
                        macroName[i++] = c;

                    else
                        token[j++] = c;
                }
                else
                {
                    state = evalToken;
                    i = j = 0;
                }
            }
            else
            {
                if (j > 0 || i > 0)
                {

                    if (state == evalToken)
                    {

                        if (isMacroOpening(token))
                        {
                            popLastToken(target, token);

                            state = parsingMacroName;
                            isMacroStartFoundYet = False;
                        }
                        else
                        {
                            if (isMacroClosing(token))
                            {
                                printf("\nisMacroClosing\ntoken:%s\n", token);
                                popLastToken(target, token);
                                addMacro(macroName, start, (ftell(source) - strlen(token) - 1));
                                memset(macroName, 0, i);
                                isMacroCurrentlyParsed = False;
                                start = end = 0;
                            }

                            else if (isPossiblyUseOfMacro(token))
                            {
                                Item *p = getMacro(token);
                                if (p != NULL)
                                {
                                    popLastToken(target, token);
                                    current = ftell(source);
                                    fseek(target, 0 - strlen(token), SEEK_CUR);
                                    replaceWithMacro(target, source, p->val.m.start, p->val.m.end);
                                    fseek(source, current - 1, SEEK_SET);
                                }
                            }

                            state = skipLine;
                        }
                    }

                    else if (state == parsingMacroName)
                    {
                        popLastToken(target, macroName);

                        if (isLegalMacroName(macroName))
                        {

                            isMacroCurrentlyParsed = True;
                            state = evalToken;
                        }
                        else
                        {
                            yieldError(illegalMacroNameUseOfSavedKeywords);
                            memset(macroName, 0, i);
                            isMacroCurrentlyParsed = isMacroStartFoundYet = False;
                            i = 0;
                            state = skipLine;
                        }
                    }
                    memset(token, 0, j);
                    i = j = 0;
                }
            }
        }
    }
}

void popLastToken(FILE *target, char *token)
{

    int len = strlen(token);
    printf("\n\ninside popLastToken,\nline:%d\ntoken:%s\nlen:%d\n", currentLine, token, len);
    fseek(target, -len, SEEK_CUR);
}
void replaceWithMacro(FILE *target, FILE *source, int start, int end)
{
    int c, i;
    fseek(source, start, SEEK_SET);
    fseek(target, 0, SEEK_CUR);

    for (i = end - start; i > 0 && (c = fgetc(source)) != EOF; i--)
        fputc(c, target);
}

FILE *createExpandedSourceFile(FILE *source, char *fileName)
{
    FILE *target;
    /*     int c = 0; */
    fileName[strlen(fileName) - 1] = 'm';
    target = fopen(fileName, "w+");
    if (target == NULL)
    {
        fclose(source);
        printf("Press any key to exit...\n");
        exit(1);
    }
    parseAndReplaceMacros(source, target);
    printf("File copied successfully.\n");
    fclose(source);
    return target;
}
