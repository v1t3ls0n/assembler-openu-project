#include "data.h"
static void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;
static void (*resetCurrentLineCounter)() = &resetCurrentLineNumber;
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

    while ((*s == ',' || isspace(*s)) && *s != '\0')
    {
        if (*s == ',')
            commasCounter++;
        s++;
    }
    if (!*s && commasCounter > 0)
        return yieldError(wrongCommasSyntaxIllegalApearenceOfCommasInLine);
    else if (*s && strlen(s) && commasCounter > 0)
        isValid = yieldError(illegalApearenceOfCommaBeforeFirstParameter);

    commasCounter = 0;
    isFirstToken = True;
    while (s && *s != '\0')
    {
        if (insideToken)
        {

            if (commasCounter > 1)
            {
                isValid = yieldError(wrongCommasSyntaxExtra);
                commasCounter = 1;
            }
            else if (commasCounter < 1 && !isFirstToken)
                isValid = yieldError(wrongCommasSyntaxMissing);

            if (isFirstToken == True)
                isFirstToken = False;

            while (*s != '\0' && !isspace(*s) && *s != ',')
                s++;

            if (*s == ',' || isspace(*s))
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

            if (*s && (isprint(*s) && !isspace(*s)))
                insideToken = True;
        }

        s++;
    }

    if (commasCounter)
        isValid = yieldError(illegalApearenceOfCommaAfterLastParameter);

    return isValid;
}

Bool countAndVerifyStringArguments(char *line)
{
    char *args, *closing = 0, *opening = 0;
    int size = 0;
    args = strstr(line, STRING) + strlen(STRING);
    args = trimFromLeft(args);
    if (!*args)
        return yieldError(emptyStringDeclatretion);

    opening = strchr(args, '\"');

    if (!opening || !*opening)
    {
        yieldError(expectedQuotes);
        yieldError(closingQuotesForStringIsMissing);
        return False;
    }
    else
    {
        closing = strrchr(args, '\"');
        if (opening == closing && (opening[0] == args[0]))
            return yieldError(closingQuotesForStringIsMissing);

        if (opening == closing && (opening[0] != args[0]))
            return yieldError(expectedQuotes);
        else
        {
            size = strlen(opening) - strlen(closing);
            increaseDataCounter(size);
        }
    }

    return True;
}

Bool parseLine(char *token, char *line)
{
    State (*globalState)() = &getGlobalState;
    Bool isValid = True;

    if (isComment(token))
        return True;

    if (isLabelDeclaration(token))
    {
        char *next = 0;
        if (!isLabelDeclarationStrict(token))
        {
            char lineClone[MAX_LINE_LEN] = {0}, *rest = 0;
            strcpy(lineClone, line);
            isValid = yieldError(missingSpaceBetweenLabelDeclaretionAndInstruction);
            token = line;
            next = strchr(line, ':');
            next++;
            *next = '\0';
            rest = strchr(lineClone, ':');
            rest++;
            sprintf(line, "%s%c%s", token, ' ', rest);
            strncpy(lineClone, line, strlen(line));
            next = (*globalState)() == firstRun ? strtok(lineClone, " \t\n\f\r") : strtok(lineClone, ", \t\n\f\r");
            return parseLine(next, line) && False;
        }
        else
        {
            next = (*globalState)() == firstRun ? strtok(NULL, " \t\n\f\r") : strtok(NULL, ", \t\n\f\r");
            if (!next)
                return yieldError(emptyLabelDecleration);

            if ((*globalState)() == firstRun)
                return handleLabel(token, next, line) && isValid;
            else
                return isValid && parseLine(next, line + strlen(token) + 1);
        }
    }

    else if (isInstruction(token))
    {
        char *next;
        int type;
        type = getInstructionType(token);
        if (!isInstructionStrict(token))
        {
            isValid = yieldError(missinSpaceAfterInstruction);
            token = getInstructionName(token);
        }
        next = (*globalState)() == firstRun ? strtok(NULL, " \t\n\f\r") : strtok(NULL, ", \t\n\f\r");

        if (isValid && next == NULL)
        {
            if (type == _TYPE_DATA || type == _TYPE_STRING)
                return type == _TYPE_DATA ? yieldWarning(emptyDataDeclaretion) : yieldError(emptyStringDeclatretion);
            else
                return type == _TYPE_ENTRY ? yieldWarning(emptyEntryDeclaretion) : yieldWarning(emptyExternalDeclaretion);
        }
        else if (next != NULL)
        {

            if ((*globalState)() == firstRun)
                return handleInstruction(type, token, next, line) && isValid;
            else
            {
                if (type == _TYPE_DATA)
                    return writeDataInstruction(next) && isValid;
                else if (type == _TYPE_STRING)
                    return writeStringInstruction(next) && isValid;
                else
                    return True;
            }
        }
    }

    else if (isOperation(token))
    {
        char args[MAX_LINE_LEN] = {0};
        strcpy(args, (line + strlen(token)));
        return (*globalState)() == firstRun ? handleOperation(token, args) : writeOperationBinary(token, args);
    }

    else
    {
        if (strlen(token) > 1)
            return yieldError(undefinedTokenNotOperationOrInstructionOrLabel);
        else
            return yieldError(illegalApearenceOfCharacterInTheBegningOfTheLine);
    }

    return isValid;
}

Bool handleSingleLine(char *line)
{
    State (*globalState)() = &getGlobalState;
    char lineCopy[MAX_LINE_LEN] = {0};
    Bool result = True;
    char *token;
    strcpy(lineCopy, line);
    token = ((*globalState)() == firstRun) ? strtok(lineCopy, " \t\n\f\r") : strtok(lineCopy, ", \t\n\f\r");
    result = parseLine(token, line);
    (*currentLineNumberPlusPlus)();
    return result;
}
void parseAssemblyCode(FILE *src)
{
    State (*globalState)() = &getGlobalState;
    void (*setState)(State) = &setGlobalState;
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

        if (isspace(c) && i > 0)
            line[i++] = ' ';

        else if (!isspace(c))
            line[i++] = c;

        if (i >= MAX_LINE_LEN - 2)
            c = '\n';

        if (c == '\n')
        {
            if (i > 0)
            {
                isValidCode = handleSingleLine(line) && isValidCode;
                memset(line, 0, MAX_LINE_LEN);
                i = 0;
            }
        }
    }

    if (i > 0)
        isValidCode = handleSingleLine(line) && isValidCode;

    if (!isValidCode)
        nextState = assemblyCodeFailedToCompile;
    else
        nextState = (*globalState)() == firstRun ? secondRun : exportFiles;

    (*resetCurrentLineCounter)();
    (*setState)(nextState);
}