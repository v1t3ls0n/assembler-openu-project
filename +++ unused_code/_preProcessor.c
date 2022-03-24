

#include "data.h"
extern void parseAssemblyCode(assemblyCode *fptrs);

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
void popCharacterFromEnd(FILE *fp, long amount);
void handleMacros(FILE *source, FILE *target);

extern char *trimFromLeft(char *s);
extern char *trimFromRight(char *s);

void copyToNewFile(FILE *source, FILE *target);

void copyToNewFile(FILE *source, FILE *target)
{
    int c;
    while ((c = fgetc(source)) != EOF)
    {
        fputc(c, target);
    }
}
void handleMacros(FILE *source, FILE *target)
{
    /*    ParseState state = parsingToken;
       char line[MAX_LINE_LEN + 1] = {0};
       char macroName[MAX_LABEL_LEN + 1] = {0};
       char *s = 0, *e = 0, *first, *token;
       int start = 0, end = 0;
       int *c, offset = 0, i = 0;

       while ((fgets(line, MAX_LINE_LEN, source)) != NULL)
       {
           token = strtok(line, " \t \n");

                   i = strlen(token) - 1;
                   c = token[i];
                   printf("c:%d\n", c);

                   while (i > 0 && isprint(token[i]) && !isspace(token[i]))
                   {
                       i--;
                       offset++;
                       ungetc(c, source);
                   }
                   printf("offset:%d\n", offset);
                   fgets(token, offset, source);
                   printf("token clean:%s\n", token);
                   offset = 0;
    s = token;
    s = trimFromLeft(s);
    state = evaluateToken(s, state);
    if ((state == writeToTargetFile || state == skipLine))
    {
        fputs(token, target);
                    memset(token, 0, MAX_LINE_LEN + 1);
}
else
{
    fseek(source, strlen(s), SEEK_CUR);

    if (parsingMacroName)
    {
        s = strstr(s, "macro ");

        fseek(source, strlen(s), SEEK_CUR);
        start = ftell(source);
        state = parsingMacroName;
        s = trimFromLeft(s);
        if (*s && strlen(s))
        {
            sscanf(s, "%s", macroName);
            start += strlen(macroName);
            s = 0;
            state = parsingMacroContent;
        }
        if ((s = strstr(s, "endm ")) != NULL)
        {
            fseek(source, strlen(token) - strlen(s), SEEK_CUR);
            end = ftell(source);
            addMacro(macroName, start, end);
            start = end = 0;
            s = 0;
            state = skipLine;
        }
    }
}
*/
}

ParseState evaluateToken(char *token, ParseState state)
{

    printf("token:%s\n", token);

    if (state == writeToTargetFile)
    {
        if (strstr(token, "macro ") == NULL)
            return writeToTargetFile;
        else
            return parsingMacroName;
    }

    else if (state == parsingToken)
    {
        if (isMacroOpening(token))
            return parsingMacroName;

        if (isPossiblyUseOfMacro(token))
        {
            Item *p = getMacro(token);
            if (p != NULL)
                return replaceMacro;
        }
        else
            return skipLine;
    }
    else if (state == parsingMacroContent)
    {
        if (isMacroClosing(token))
            return addMacroToTable;
        else
            return parsingMacroContent;
    }

    else if (state == parsingMacroName)
    {
        if (isLegalMacroName(token))
            return parsingMacroContent;

        else
        {
            yieldError(illegalMacroNameUseOfSavedKeywords);
            return Err;
        }
    }

    return skipLine;
}

void popLastToken(FILE *target, char *token)
{
    int len = strlen(token);
    printf("\n\ninside popLastToken,\nline:%d\ntoken:%s\nlen:%d\n", (*currentLine)(), token, len);
    fseek(target, -len, SEEK_CUR);
}

void popCharacterFromEnd(FILE *fp, long amount)
{
    int c;

    while ((c = fgetc(fp)) != EOF && amount > 0)
    {
        fseek(fp, -1L, SEEK_CUR);
        fputc(' ', fp);
        amount--;
    }
}
void removeCharactersWithinRange(FILE *fp, long start, long end)
{
    int c = 0, offset = end - start;
    /*     fseek(fp, start, SEEK_SET); */
    fseek(fp, -offset, SEEK_END);
    while ((c = fgetc(fp)) != EOF && start < end)
    {
        fputc(0, fp);
        start++;
        /*      fprintf(fp, "%c", 0); */
        /*   offset--; */
    }
}

void removeLastMacroDeclarition(FILE *fp)
{
    /*     int c = 0, offset = end - start;

        fseek(fp, start, SEEK_END);
        while ((c = fgetc(fp)) != EOF && start < end)
        {
            fputc(0, fp);
            start++;

        } */
}

long getLineStart(FILE *fp, unsigned direction)
{
    long result = 0;
    /*     int c;
        long current = ftell(fp) - 1, result;
        while ((c = fgetc(fp)) != EOF && c != '\n')
        {
            if (direction)
                printf("search next new line\n");
            else
                printf("search previous new line\n");

            fseek(fp, direction ? 1L : -1L, direction ? SEEK_CUR : SEEK_END);
        }
        result = ftell(fp) - 1;

            fseek(fp, current, SEEK_SET); */
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

    /*     parseAndReplaceMacros(source, target); */
    copyToNewFile(source, target);
    /*  fclose(source); */
    rewind(source);
    rewind(target);
    parseAssemblyCode(target);
    /*     rewind(target); */

    /*     printMacroTable(); */
    /*     handleMacros(source, target); */
    /*     updateGlobalState(firstRun); */
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
                return parsingMacroName;

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

    case parsingMacroName:
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

void parseAndReplaceMacros(FILE *source, FILE *target)
{
    /*
 ParseState state = parsingToken;
 int toDeleteStart = 0, toDeleteEnd = 0;
 long start = 0, end = 0;
 int c = 0, i = 0, j = 0;
 size_t start = 0;
 char *s = 0, *e = 0;
 long offset = 0;

     while ((c = fgetc(source)) != EOF)
     {
         start = ftell(source);
         fgets(token, MAX_LINE_LEN, source);
         state = evaluateToken(token, state);

                 if ((s = strstr(token, "macro")) != NULL)
                 {
                     start += strlen(token) - (strlen(token) - strlen(s) + 6);
                 }
                 if (!strstr(token, "macro") && !strstr(token, "endm"))
                 {
                 }

                 if (state != skipLine)
                 {
                     if (!isspace(c))
                     {

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

                         if (state == parsingMacroName)
                             macroName[i++] = c;
                         else
                             token[j++] = c;
                     }
                     else if (isspace(c) && (i > 0 || j > 0))
                     {
                         if (state == parsingMacroName)
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
                     case parsingMacroName:
                     {

                         printf("get Macro Name case\n");

                                        toDeleteStart = getLineStart(target, 0) - 1;

         break;
     }

 case parsingMacroContent:
 {
     start = ftell(source);

     printf("reading Macro case\n");
 }
 case addMacroToTable:
 {

     end = ftell(source);
     addMacro(macroName, start, end);
     memset(macroName, 0, MAX_LABEL_LEN);
     start = end = i = j = 0;
     state = skipLine;
     break;
 }
 case replaceMacro:
 {

     printf("replace macro case\n");
     break;
 }
 case parsingToken:
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
 else
 {
     popCharacterFromEnd(target, offset);
     offset = 0;
 }
 }
 */
}
