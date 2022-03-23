

#include "data.h"
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
extern void updateGlobalState(State new);

extern Bool isPossiblyUseOfMacro(char *s);
extern Bool isMacroOpening(char *s);
extern Bool isMacroClosing(char *s);
extern Bool isLegalMacroName(char *s);

void saveMacros(FILE *target);
void popLastToken(FILE *target, char *token, int offset);
void parseAndReplaceMacros(FILE *source, FILE *target);
void replaceWithMacro(FILE *target, FILE *source, int start, int end);
void createExpandedSourceFile(FILE *source, FILE *target, char *fileName);
void popCharacters(FILE *target, fpos_t position, int amount);

void parseAndReplaceMacros(FILE *source, FILE *target)
{
    void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;
    ParseState state = evalToken;
    /*     Bool isMacroCurrentlyParsed = False;
        Bool isMacroStartFoundYet = False; */
    int c = 0, i = 0, j = 0, offsetCounter = 0, current = 0;
    int start = 0, end = 0;

    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};

    while ((c = fgetc(source)) != EOF)
    {
        offsetCounter++;

        if (c == '\n')
        {
            (*currentLineNumberPlusPlus)();
            if ((state == findStartOfMacroContent || state == parsingMacroName))
            {
                start = ftell(source) - 1;
                state = findEndOfMacroContent;
            }
            else if (state == findEndOfMacroContent && j > 0)
            {
                if (isMacroClosing(token))
                {
                    end = ftell(source) - offsetCounter;
                    printf("inside macro closing\n");
                    addMacro(macroName, start, end);
                    memset(macroName, 0, MAX_LABEL_LEN);
                    offsetCounter = j = i = start = end = 0;
                    state = skipLine;
                }
            }
            if (state == skipLine)
                state = evalToken;
        }

        if (isspace(c) && c != '\n' && state != skipLine)
        {
            /* printf("\n\ninside is space and not new line\n");
            printf("token:%s\nmacroName:%s\n\n", token, macroName); */

            if ((j > 0 || i > 0))
            {
                if (state == findEndOfMacroContent)
                {
                    if (isMacroClosing(token))
                    {
                        printf("inside macro closing\n");
                        addMacro(macroName, start, end);
                        memset(macroName, 0, MAX_LABEL_LEN);
                        offsetCounter = j = i = start = end = 0;
                    }
                }

                if (state == parsingMacroName)
                {
                    if (isLegalMacroName(macroName))
                    {
                        printf("inside legal macro name, macro name is :%s\n", macroName);

                        state = findStartOfMacroContent;
                    }
                    /*                 else
                                    {
                                        yieldError(illegalMacroNameUseOfSavedKeywords);
                                        memset(macroName, 0, MAX_LABEL_LEN);
                                        start = end = i = 0;
                                        state = skipLine;
                                    } */
                }

                else if (state == evalToken)
                {
                    if (isMacroOpening(token))
                    {
                        printf("inside is macro opening\n");
                        state = parsingMacroName;
                    }

                    else if (isPossiblyUseOfMacro(token))
                    {
                        printf("inside isPossiblyUseOfMacro\n");

                        Item *p = getMacro(token);
                        if (p != NULL)
                        {
                            /*               popLastToken(target, "", offsetCounter);
                                          offsetCounter = 0; */
                            /*                         current = ftell(source) - 1;
                                                    fseek(target, -offsetCounter, SEEK_CUR);
                                                    replaceWithMacro(target, source, p->val.m.start, p->val.m.end);
                                                    fseek(source, current + offsetCounter, SEEK_SET); */
                        }
                        state = skipLine;
                    }
                    else
                        state = skipLine;
                }
                memset(token, 0, MAX_LABEL_LEN);
                j = 0;
            }

            /*   offsetCounter = 0; */
        }
        else if (!isspace(c))
        {
            if (state == parsingMacroName)
                macroName[i++] = c;
            if (state == evalToken)
                token[j++] = c;
        }

        fputc(c, target);
    }

    /*
        if (offsetCounter && (state == evalToken || state == parsingMacroName))
        {
            if (isMacroOpening(token) || isMacroClosing(token))
            {
                popLastToken(target, "", offsetCounter - j - i);
            }

            else if (isPossiblyUseOfMacro(token))
            {
                Item *p = getMacro(token);
                popLastToken(target, token, offsetCounter - j - i);
                if (p != NULL)
                    replaceWithMacro(target, source, p->val.m.start, p->val.m.end);
            }
        } */
}

void popCharacters(FILE *target, fpos_t position, int amount)
{ /*
    int c = 0;
        fsetpos(target, &position);
        while (amount-- && (c = fgetc(target)) != EOF)
        {

            printf("line 184\n");
            fputc(' ', target);
        }
     */
}

void popLastToken(FILE *target, char *token, int offset)
{

    /*     int len = strlen(token); */
    /*     printf("\n\ninside popLastToken,\nline:%d\ntoken:%s\nlen:%d\n", , token, len);
     */
    /*     fseek(target, -len, SEEK_CUR);
        fputc(' ', target); */
    if (offset)
    {
        fseek(target, -offset, SEEK_CUR);
        while (--offset)
            fputc(' ', target);
    }
}
void replaceWithMacro(FILE *target, FILE *source, int start, int end)
{
    int c, i;
    fseek(source, start, SEEK_SET);
    fseek(target, 0, SEEK_CUR);
    for (i = end - start - 1; i > 0 && (c = fgetc(source)) != EOF; i--)
        fputc(c, target);
}

void createExpandedSourceFile(FILE *source, FILE *target, char *fileName)
{

    parseAndReplaceMacros(source, target);
    updateGlobalState(firstRun);
}