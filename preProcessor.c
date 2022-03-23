

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
void moveToLineStart(FILE *fp, unsigned direction);
long getLineStart(FILE *fp, unsigned direction);
ParseState getNextState(char *token, char *macroName, int c, ParseState currentState);
ParseState evaluateToken(char *token, ParseState state);
void removeCharactersWithinRange(FILE *fp, long start, long end);

void parseAndReplaceMacros(FILE *source, FILE *target)
{
    ParseState state = readingToken;

    int toDeleteStart = 0, toDeleteEnd = 0;
    int start = 0, end = 0;
    int c = 0, i = 0, j = 0;

    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(source)) != EOF)
    {
        fputc(c, target);
        if (c == '\n' && state == skipLine)
        {
            memset(token, 0, MAX_LINE_LEN);
            j = 0;
            state = readingToken;
        }

        if (j > MAX_LINE_LEN)
        {
            memset(token, 0, MAX_LINE_LEN);
            j = 0;
        }
        if (i > MAX_LABEL_LEN)
        {
            memset(macroName, 0, MAX_LABEL_LEN);
            i = 0;
        }

        if (state != skipLine)
        {
            if (!isspace(c))
            {
                if (state == getMacroName)
                    macroName[i++] = c;
                else
                    token[j++] = c;
            }
            else if (isspace(c) && (i > 0 || j > 0))
            {
                if (state == getMacroName)
                    state = evaluateToken(macroName, state);
                else
                {
                    state = evaluateToken(token, state);
                    memset(token, 0, MAX_LINE_LEN);
                    j = 0;
                }
            }

            switch (state)
            {
            case skipLine:
            {
                printf("skip line case\n");

                break;
            }
            case getMacroName:
            {
                printf("get Macro Name case\n");
                toDeleteStart = getLineStart(target, 0);

                break;
            }

            case readingMacro:
            {
                printf("reading Macro case\n");
                start = getLineStart(source, 1) + 1;
            }
            case addMacroToTable:
            {
                printf("add Macro To Table case\n");
                end = getLineStart(source, 0) - 1;
                toDeleteEnd = getLineStart(target, 0);
                addMacro(macroName, start, end);
                memset(macroName, 0, MAX_LABEL_LEN);
                removeCharactersWithinRange(target, toDeleteStart, toDeleteEnd);
                start = end = toDeleteEnd = toDeleteStart = i = j = 0;
                state = skipLine;

                break;
            }
            case replaceMacro:
            {
                printf("replace macro case\n");
                break;
            }
            case readingToken:
            {
                printf("reading token case\n");

                break;
            }
            default:
            {
                printf("default case\n");

                break;
            }
            }
        }
    }
}

ParseState evaluateToken(char *token, ParseState state)
{
    printf("inside evaluateToken\ntoken:%s\n", token);
    if (state == readingToken)
    {
        if (isMacroOpening(token))
            return getMacroName;

        if (isPossiblyUseOfMacro(token))
        {
            Item *p = getMacro(token);
            if (p != NULL)
                return replaceMacro;
        }
    }
    if (state == readingMacro)
    {
        if (isMacroClosing(token))
            return addMacroToTable;
        else
            return readingMacro;
    }

    if (state == getMacroName)
    {
        printf("line 158\n");
        if (isLegalMacroName(token))
            return readingMacro;
        else
        {
            yieldError(illegalMacroNameUseOfSavedKeywords);
            return Err;
        }
    }

    return skipLine;
}

/* ParseState getNextState(char *token, char *macroName, int c, ParseState currentState)
{
    printf("token:%s\n", token);

    switch (currentState)
    {
    case skipLine:
    {
        printf("line 112\n");
        if (c == '\n')
            return evalToken;
    }

    case evalToken:
    {
        printf("line 118\n");
        if (isspace(c) && strlen(token) > 0)
        {
            if (isMacroOpening(token))
                return getMacroName;

            else if (isPossiblyUseOfMacro(token))
            {
                Item *p = getMacro(token);
                if (p != NULL)
                    return replaceMacro;
            }
        }
        memset(token, 0, MAX_LINE_LEN);

        break;
    }

    case addMacroToTable:
        return deleteMacroFromTarget;
    case searchingMacroEnd:
    {
        if (isMacroClosing(token))
            return addMacroToTable;
    }

    case getMacroName:
    {
        if (strlen(macroName) && isspace(c))
        {
            if (isLegalMacroName(macroName))
                return searchingMacroEnd;
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
 */
void popLastToken(FILE *target, char *token)
{
    int len = strlen(token);
    printf("\n\ninside popLastToken,\nline:%d\ntoken:%s\nlen:%d\n", (*currentLine)(), token, len);
    fseek(target, -len, SEEK_CUR);
}
/* macro m1 */
/*  endm */

void removeCharactersWithinRange(FILE *fp, long start, long end)
{
    int c = 0, offset = end - start;
    fseek(fp, start, SEEK_SET);
    while ((c = fgetc(fp)) != EOF && offset--)
        fprintf(fp, "%c", 0);
}

long getLineStart(FILE *fp, unsigned direction)
{

    int c;
    long current = ftell(fp), result;
    while ((c = fgetc(fp)) != EOF && c != '\n')
        fseek(fp, direction ? 1L : -1L, SEEK_CUR);

    result = ftell(fp) + 1;
    fseek(fp, current, SEEK_SET);
    return result;
}

void moveToLineStart(FILE *fp, unsigned direction)
{
    int c;
    while ((c = fgetc(fp)) != EOF)
        if (c != '\n')
            fseek(fp, direction ? 1L : -1L, SEEK_CUR);
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
