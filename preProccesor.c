#include "data.h"
void (*setState)(State) = &setGlobalState;
State (*globalState)() = &getGlobalState;

void parseMacros(char *line, char *token, FILE *src, FILE *target)
{
    void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;
    static char macroName[MAX_LABEL_LEN] = {0}, *next;
    static Bool isReadingMacro = False;
    static long start = 0, end = 0;
    extern Bool isPossiblyUseOfMacro(char *s);
    extern Bool isMacroOpening(char *s);
    extern Bool isMacroClosing(char *s);
    extern Bool isLegalMacroName(char *s);
    extern Item *addMacro(char *name, int start, int end);
    extern Item *updateMacro(char *name, int start, int end);
    extern Item *getMacro(char *s);

    if (!isReadingMacro)
    {
        if (!isMacroOpening(token))
        {
            fprintf(target, "%s", line);
            (*currentLineNumberPlusPlus)();
        }
    }
    if (!isPossiblyUseOfMacro(token) && !isMacroOpening(token) && !isMacroClosing(token))
        return;

    if (isMacroOpening(token))
    {
        next = strtok(NULL, " \t\n\f\r");

        if (next == NULL)
        {
            yieldError(macroDeclaretionWithoutDefiningMacroName);
            (*setState)(assemblyCodeFailedToCompile);
            return;
        }
        if (!isLegalMacroName(next))
        {
            yieldError(illegalMacroNameUseOfSavedKeywords);
            (*setState)(assemblyCodeFailedToCompile);
            return;
        }
        start = ftell(src);
        strcpy(macroName, next);
        isReadingMacro = True;
    }
    else if (isMacroClosing(token))
    {
        end = ftell(src) - strlen(line) + 1;
        addMacro(macroName, start, end);
        isReadingMacro = False;
        start = end = 0;
        memset(macroName, 0, MAX_LABEL_LEN);
    }
    else if (isPossiblyUseOfMacro(token))
    {
        Item *p = getMacro(token);
        if (p != NULL)
        {
            long c, toCopy = p->val.m.end - p->val.m.start;
            long lastPosition = 0;
            fseek(target, -strlen(line), SEEK_CUR);
            fprintf(target, "%s", "\0");
            lastPosition = ftell(src);
            fseek(src, p->val.m.start, SEEK_SET);

            while (--toCopy && (c = fgetc(src)) != EOF)
                fputc(c, target);

            fseek(src, lastPosition, SEEK_SET);
        }
    }
}

void parseSourceFile(FILE *src, FILE *target)
{
    char line[MAX_LINE_LEN] = {0};
    char lineClone[MAX_LINE_LEN] = {0};
    char *token, c;
    int i = 0;
    void (*resetCurrentLineCounter)() = &resetCurrentLineNumber;
    (*resetCurrentLineCounter)();

    while (((c = fgetc(src)) != EOF))
    {
        line[i++] = c;

        if (i >= MAX_LINE_LEN - 2 && !isspace(c))
            c = '\n';

        if ((*globalState)() == assemblyCodeFailedToCompile)
            return;

        if (c == '\n')
        {
            if (i > 0)
            {
                strncpy(lineClone, line, i);
                token = strtok(lineClone, " \t\n\f\r");
                if (token != NULL)
                    parseMacros(line, token, src, target);

                memset(lineClone, 0, i);
                memset(line, 0, i);
                i = 0;
            }
        }
    }

    if (i > 0)
    {
        strcpy(lineClone, line);
        token = strtok(lineClone, " \t\n\f\r");
        if (token != NULL)
            parseMacros(line, token, src, target);
    }

    if ((*globalState)() != assemblyCodeFailedToCompile)
        (*setState)(firstRun);
}