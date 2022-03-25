#include "data.h"
void (*setState)() = &setGlobalState;
State (*globalState)() = &getGlobalState;

extern Bool isPossiblyUseOfMacro(char *s);
extern Bool isMacroOpening(char *s);
extern Bool isMacroClosing(char *s);
extern Bool isLegalMacroName(char *s);
extern Item *addMacro(char *name, int start, int end);
extern Item *updateMacro(char *name, int start, int end);
extern Item *getMacro(char *s);

char *getFirstToken(char *s);
char *getNthToken(char *s, int n);

Bool parseMacros(char *line, char *token, FILE *src, FILE *target)
{

    static char macroName[MAX_LABEL_LEN] = {0}, *next;
    static Bool isReadingMacro = False;
    static long start = 0, end = 0;

    if (!isReadingMacro)
        fprintf(target, "%s\n", line);

    if (isMacroOpening(token))
    {

        printf("is macro opening!\n");
        next = strtok(NULL, " \t \n");
        if (!*next)
            return yieldError(macroDeclaretionWithoutDefiningMacroName);
        if (!isLegalMacroName(next))
            return yieldError(illegalMacroNameUseOfSavedKeywords);

        start = ftell(src) + strlen(token) + strlen(next) - 1;
        strcpy(macroName, next);
        isReadingMacro = True;
        fseek(target, -strlen(line) - 1, SEEK_END);
    }
    else if (isMacroClosing(token))
    {
        end = ftell(src) - strlen(token) - 1;
        printf("is macro closing!\nend:%d\n", (int)end);
        addMacro(macroName, start, end);
        isReadingMacro = False;
        start = end = 0;
        memset(macroName, 0, MAX_LABEL_LEN);
    }
    else if (isPossiblyUseOfMacro(token))
    {
        Item *p = getMacro(token);
        printf("is possibly use of macro!\n");
        if (p != NULL)
        {
            int c, toCopy = p->val.m.end - p->val.m.start, toDelete = strlen(token);
            long lastPosition;
            lastPosition = ftell(src);
            fseek(src, p->val.m.start, SEEK_SET);
            fseek(target, -toDelete - 1, SEEK_CUR);
            while (toCopy > 0)
            {
                c = fgetc(src);
                fputc(c, target);
                --toCopy;
            }

            fseek(src, lastPosition, SEEK_SET);
        }
    }

    return True;
}

void parseSourceFile(FILE *src, FILE *target)
{
    char line[MAX_LINE_LEN + 2] = {0};
    char lineClone[MAX_LINE_LEN + 2] = {0};
    char *token, c;
    int i = 0;
    void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;

    while (((c = fgetc(src)) != EOF))
    {
        if (i >= MAX_LINE_LEN - 1 && !isspace(c))
        {
            memset(line, 0, MAX_LINE_LEN);
            i = 0;
        }

        if (c == '\n' && i > 0)
        {
            (*currentLineNumberPlusPlus)();
            memcpy(lineClone, line, MAX_LINE_LEN);
            token = strtok(lineClone, " \t \n");
            if (!parseMacros(line, token, src, target))
            {
                (*setState)(assemblyCodeFailedToCompile);
                return;
            }
            memset(line, 0, MAX_LINE_LEN);
            i = 0;
        }
        if (isspace(c) && i > 0)
            line[i++] = ' ';

        else if (isprint(c) && !isspace(c))
            line[i++] = c;
    }
    if (i > 0)
        parseMacros(line, token, src, target);

    (*setState)(firstRun);
}

char *getNthToken(char *s, int n)
{
    char *token = (char *)calloc(strlen(s), sizeof(char *));
    int i = 0;
    while (!*s)
    {
        if (isspace(*s))
            i++;
        if (i == n && !isspace(*s))
            *token = *s++;
    }
    token = (char *)realloc(token, strlen(token) * sizeof(char *));
    return token;
}

char *getFirstToken(char *s)
{
    char *firstToken = (char *)calloc(strlen(s), sizeof(char *));
    while (!isspace(*s) && isprint(*s))
    {
        *firstToken = *s++;
    }
    firstToken = (char *)realloc(firstToken, strlen(firstToken) * sizeof(char *));
    return firstToken;
}