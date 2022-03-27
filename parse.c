#include "data.h"
#include <stdarg.h>

extern Bool yieldError(Error err);
extern Bool yieldWarning(Warning err);
extern char *trimFromLeft(char *s);
extern int countConsecutiveCommas(char *s);
extern int countLengthOfNonDigitToken(char *s);
static void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;
extern FILE *getSourceFilePointer();
static void (*resetCurrentLineCounter)() = &resetCurrentLineNumber;
Bool verifyCommaSyntax(char *line);
Bool handleSingleLine(char *line);

/* @ Function: countAndVerifyDataArguments
   @ Arguments: the function get char * line which is the current line that we are about to parse the data arguments from.
   @ Description: The function extracts the argument string of the .data instruction, than the function analyses\ parses the string.
   If the function encounter errors or illegal arguments, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
   While the function parsing the arguments, ir also counts the number of .data elements that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .data instruction will take.
*/

Bool countAndVerifyDataArguments(char *line)
{
    Bool isValid = True;
    int size = 0, n = 0, num = 0;
    char c = 0;
    char args[MAX_LINE_LEN + 1] = {0}, *p;
    line = strstr(line, DATA) + strlen(DATA);

    /* we make the pointer p to point on the position of the first character coming sfter the .data
     instruction within the full line, so that p will point on the begining of the arguments string*/

    /*copies the string of arguments pointer by p into the args local string we will use for parsing*/
    strcpy(args, line);

    isValid = verifyCommaSyntax(args);
    p = strtok(line, ", \t\n\f\r");

    while (p != NULL)
    {
        sscanf(p, "%d%n%c", &num, &n, &c);
        if (c == '.' && n > 0)
            isValid = yieldError(wrongArgumentTypeNotAnInteger);
        num = atoi(p);
        if (!num && *p != '0')
            isValid = yieldError(expectedNumber);

        n = num = c = 0;
        size++;
        p = strtok(NULL, ", \t\n\f\r");
    }

    printf("line:%s size:%d\n", line, size);
    if (isValid)
        increaseDataCounter(size);

    return isValid;
}

Bool verifyCommaSyntax(char *line)
{
    int commasCounter = 0;
    Bool insideToken = False;
    Bool isFirstToken = True;
    Bool isValid = True;
    char *s = line;
    s = trimFromLeft(s);
    printf("line in verifyCommaSyntax:%s\n", line);

    while (*s == ',' || isspace(*s))
    {
        if (*s == ',')
            commasCounter++;
        s++;
    }

    if (commasCounter > 0)
        isValid = yieldError(illegalApearenceOfCommaBeforeFirstParameter);

    commasCounter = 0;
    isFirstToken = True;
    while (s && *s != '\0')
    {
        if (insideToken)
        {
            if (isFirstToken == True && commasCounter == 0)
            {
                commasCounter = 1;
                isFirstToken = False;
            }

            if (commasCounter > 1)
            {
                isValid = yieldError(wrongOperationSyntaxExtraCommas);
                commasCounter = 1;
            }
            else if (commasCounter < 1)
            {
                isValid = yieldError(wrongOperationSyntaxMissingCommas);
                commasCounter = 1;
            }
            if (s && isspace(*s))
            {
                insideToken = False;
                commasCounter = 0;
            }
            else if (*s == ',')
            {
                insideToken = False;
                commasCounter = 0;
                s--;
            }
        }
        else
        {
            while (*s == ',' || isspace(*s))
            {
                if (*s == ',')
                    commasCounter++;
                s++;
            }

            if (s && (isprint(*s) && !isspace(*s)))
                insideToken = True;
        }

        s++;
    }

    if (commasCounter > 0)
        isValid = yieldError(illegalApearenceOfCommaAfterLastParameter);

    printf("are commas valid? %d\n", isValid);
    return isValid;
}

Bool countAndVerifyStringArguments(char *line)
{
    char *s = 0, *args;
    int size = 0;
    args = strchr(line, '\"');
    if (!*args)
        return yieldWarning(emptyStringDeclatretion);

    if (args[0] != '\"')
        return yieldError(expectedQuotes);

    s = strrchr(args, '\"');
    while (*s && *s != '\0')
    {
        if (!isspace(*s) && isprint(*s) && *s != '\"')
            return yieldError(closingQuotesForStringIsMissing);
        s++;
        size++;
    }

    increaseDataCounter((int)(size)); /*counts the \0 at the end of the string as well*/

    return True;
}

ParseState parseLine(char *token, char *line)
{
    State (*globalState)() = &getGlobalState;

    if (isComment(token))
        return lineParsedSuccessfully;

    if (isLabelDeclaration(token))
    {
        if (strlen(token) == 1)
            yieldError(illegalLabelDeclaration);
        else
        {
            char *next = (*globalState)() == firstRun ? strtok(NULL, " \t\n\f\r") : strtok(NULL, ", \t\n\f\r");
            if (!next)
                return yieldError(emptyLabelDecleration);

            if ((*globalState)() == firstRun)
                return handleLabel(token, next, line) ? lineParsedSuccessfully : Err;
            else
                return parseLine(next, line + strlen(token) + 1);
        }
    }

    else if (isInstruction(token))
    {
        char *next;
        int type;
        Bool isValid = True;

        if (!isInstructionStrict(token))
        {
            isValid = yieldError(missinSpaceAfterInstruction);
            token = getInstructionName(token);
        }
        type = getInstructionType(token);
        next = (*globalState)() == firstRun ? strtok(NULL, " \t\n\f\r") : strtok(NULL, ", \t\n\f\r");
        printf("line:%s\ntoken:%s\nnext:%s\n", line, token, next);

        if (next == NULL)
        {
            printf("line 328\n");
            if (type == _TYPE_DATA || type == _TYPE_STRING)
                return type == _TYPE_DATA ? yieldWarning(emptyDataDeclaretion) : yieldWarning(emptyStringDeclatretion);
            else
                return type == _TYPE_ENTRY ? yieldWarning(emptyEntryDeclaretion) : yieldWarning(emptyExternalDeclaretion);
        }
        else
        {
            if ((*globalState)() == firstRun)
                return isValid && handleInstruction(type, token, next, line);
            else
            {
                if (type == _TYPE_DATA)
                    return writeDataInstruction(next) ? lineParsedSuccessfully : Err;
                else if (type == _TYPE_STRING)
                    return writeStringInstruction(next) ? lineParsedSuccessfully : Err;
                else
                    return lineParsedSuccessfully;
            }
        }
    }

    else if (isOperation(token))
    {
        char args[MAX_LINE_LEN] = {0};
        strcpy(args, (line + strlen(token)));
        return (*globalState)() == firstRun ? handleOperation(token, args) : writeOperationBinary(token, args) ? lineParsedSuccessfully
                                                                                                               : Err;
    }

    else
    {
        if (strlen(token) > 1)
            yieldError(undefinedTokenNotOperationOrInstructionOrLabel);
        else
            yieldError(illegalApearenceOfCharacterInTheBegningOfTheLine);
    }

    return Err;
}

Bool handleSingleLine(char *line)
{
    State (*globalState)() = &getGlobalState;
    ParseState state = newLine;
    char lineCopy[MAX_LINE_LEN] = {0};
    char *token;
    strcpy(lineCopy, line);
    printf("line:%s\n", line);
    token = ((*globalState)() == firstRun) ? strtok(lineCopy, " \t\n\f\r") : strtok(lineCopy, ", \t\n\f\r");
    state = parseLine(token, line);
    (*currentLineNumberPlusPlus)();
    return state == lineParsedSuccessfully
               ? True
               : False;
}

void parseAssemblyCode(FILE *src)
{
    State (*globalState)() = &getGlobalState;
    void (*setState)() = &setGlobalState;
    int c = 0, i = 0;
    char line[MAX_LINE_LEN] = {0};

    Bool isValidCode = True;
    State nextState;
    char *(*fileName)() = &getFileNamePath;

    (*resetCurrentLineCounter)();
    if ((*globalState)() == secondRun)
        printf("\n\n\nSecond Run:(%s)\n", (*fileName)());
    else if ((*globalState)() == firstRun)
        printf("\n\n\nFirst Run:(%s)\n", (*fileName)());

    while (((c = fgetc(src)) != EOF))
    {
        putchar(c);

        if (i >= MAX_LINE_LEN - 1 && !isspace(c))
        {
            isValidCode = yieldError(maxLineLengthExceeded);
            memset(line, 0, i);
            i = 0;
        }

        if (isspace(c) && i > 0)
        {
            line[i++] = ' ';
        }

        else if (isprint(c) && !isspace(c))
        {
            line[i++] = c;
        }

        if (c == '\n')
        {
            line[i++] = '\n';
            if (i > 0)
            {
                isValidCode = handleSingleLine(line) && isValidCode;
                memset(line, 0, i);
                i = 0;
            }
        }
    }

    if (i > 0)
    {
        isValidCode = handleSingleLine(line) && isValidCode;
    }

    if (!isValidCode)
        nextState = assemblyCodeFailedToCompile;
    else
        nextState = (*globalState)() == firstRun ? secondRun : exportFiles;

    (*resetCurrentLineCounter)();
    (*setState)(nextState);
}