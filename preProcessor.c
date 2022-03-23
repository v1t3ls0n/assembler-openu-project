

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

extern Bool isPossiblyUseOfMacro(char *s);
extern Bool isMacroOpening(char *s);
extern Bool isMacroClosing(char *s);
extern Bool isLegalMacroName(char *s);

void saveMacros(FILE *target);
void popLastToken(FILE *target, char *token);
void parseAndReplaceMacros(FILE *source, FILE *target);
void replaceWithMacro(FILE *target, FILE *source, int start, int end);
extern void updateGlobalState(State new);
void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;
int (*currentLine)() = &getCurrentLineNumber;

long popLastLine(FILE *target, int start);
void moveToLineStart(FILE *fp);
long getLineStart(FILE *fp);

ParseState getNextState(char *token, char *macroName, int c, ParseState currentState);
void parseAndReplaceMacros(FILE *source, FILE *target)
{
    ParseState state = evalToken;
    Bool isMacroCurrentlyParsed = False;
    Bool isMacroStartFoundYet = False;

    int toDeleteStart = 0, toDeleteEnd = 0;
    int start = 0, end = 0;
    int c = 0, i = 0, j = 0, current = 0;
    long lastLineStart = ftell(source) - 1;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(source)) != EOF)
    {
        /*     if (!isMacroCurrentlyParsed) */

        fputc(c, target);

        state = getNextState(token, macroName, c, state);

        switch (state)
        {

        case evalToken:
        {
            if (!isspace(c))
                token[j++] = c;
            break;
        }

        case parsingMacroName:
        {
            if (!isspace(c))
                macroName[i++] = c;
            break;
        }
        case skipLine:
        {
            memset(token, 0, MAX_LINE_LEN);
            i = 0;
        }

        case searchingMacroStart:
        {
            toDeleteStart = getLineStart(target);
        }

        case searchingMacroEnd:
        {
            start = ftell(source);
        }

        case addMacroToTable:
        {
            end = getLineStart(source) - 1;
            addMacro(macroName, start, end);
            memset(macroName, 0, MAX_LABEL_LEN);
            start = end = i = 0;
        }
        case deleteMacroFromTarget:
        {
        }
        }
    }
}

ParseState getNextState(char *token, char *macroName, int c, ParseState currentState)
{

    switch (currentState)
    {
    case skipLine:
    {
        if (c == '\n')
            return evalToken;
    }
    case evalToken:
    {
        if (isspace(c) && strlen(token) > 0)
        {

            if (isMacroOpening(token))
                return parsingMacroName;
            else if (isPossiblyUseOfMacro(token))
            {
                Item *p = getMacro(token);
                if (p != NULL)
                    return replaceMacro;
            }
            else
                return skipLine;
        }

        break;
    }

    case addMacroToTable:
        return deleteMacroFromTarget;
    case searchingMacroEnd:
    {
        if (isMacroClosing(token))
            return addMacroToTable;
    }
    case searchingMacroStart:
    {
        if (c == '\n')
            return searchingMacroEnd;
    }
    case parsingMacroName:
    {
        if (strlen(macroName) && isspace(c))
        {

            if (isLegalMacroName(macroName))
            {
                if (c == '\n')
                    return searchingMacroEnd;
                else
                    return searchingMacroStart;
            }
            else
            {
                yieldError(illegalMacroNameUseOfSavedKeywords);
                return Err;
            }
        }
    }

    default:
        break;
    }

    return currentState;
}

void popLastToken(FILE *target, char *token)
{
    int len = strlen(token);
    printf("\n\ninside popLastToken,\nline:%d\ntoken:%s\nlen:%d\n", (*currentLine)(), token, len);
    fseek(target, -len, SEEK_CUR);
}
/* macro m1 */
/*  endm */

long getLineStart(FILE *fp)
{

    int c;
    long current = ftell(fp), result;
    while ((c = fgetc(fp)) != EOF && c != '\n')
        fseek(fp, -1L, SEEK_CUR);

    result = ftell(fp) + 1;
    fseek(fp, current, SEEK_SET);
    return result;
}

void moveToLineStart(FILE *fp)
{
    int c;
    while ((c = fgetc(fp)) != EOF)
        if (c != '\n')
            fseek(fp, -1L, SEEK_CUR);
}

long popLastLine(FILE *target, int start)
{
    int c;
    fseek(target, start, SEEK_SET);
    while ((c = fgetc(target)) != EOF)
        if (c == '\n')
            break;
        else
            fputc(' ', target);

    return ftell(target) - 1;
}

void replaceWithMacro(FILE *target, FILE *source, int start, int end)
{
    int c, i;
    fseek(source, start, SEEK_SET);
    fseek(target, 0, SEEK_CUR);

    for (i = end - start; i > 0 && (c = fgetc(source)) != EOF; i--)
        fputc(c, target);
}

void createExpandedSourceFile(FILE *source, FILE *target, char *fileName)
{

    parseAndReplaceMacros(source, target);
    updateGlobalState(firstRun);
}

/*
        if (isMacroCurrentlyParsed)
            fseek(target, popLastLine(target, lastLineStart), SEEK_SET);

        if (c == '\n')
        {
            lastLineStart = ftell(target);
            if (isMacroCurrentlyParsed && !isMacroStartFoundYet)
            {
                start = ftell(source) - 1;
                isMacroStartFoundYet = True;
            }
            (*currentLineNumberPlusPlus)();
            if (state == skipLine)
                state = evalToken;

            lastLineStart = ftell(source) - 1;
        }

        if (state != skipLine)
        {

            if (!isspace(c))
            {

                if (j < MAX_LINE_LEN && i < MAX_LABEL_LEN)
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
                     
else
{
    if (isMacroCurrentlyParsed && isMacroClosing(token))
    {

        lastLineStart = popLastLine(target, lastLineStart);
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

            lastLineStart = popLastLine(target, lastLineStart);
            current = ftell(source) - 1;
            fseek(target, lastLineStart, SEEK_CUR);
            replaceWithMacro(target, source, p->val.m.start, p->val.m.end);
            fseek(source, current, SEEK_SET);
        }
    }

    state = skipLine;
}
}

else if (state == parsingMacroName)
{

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

* /