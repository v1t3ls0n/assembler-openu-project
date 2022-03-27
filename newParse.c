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
Bool isLabelDeclarationStrict(char *s);

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
    /* we make the pointer p to point on the position of the first character coming sfter the .data
    instruction within the full line, so that p    will point on the begining of the arguments string*/
    line = strstr(line, DATA) + strlen(DATA);
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

    while (*s == ',' || isspace(*s))
    {
        if (*s == ',')
            commasCounter++;
        s++;
    }

    if (*s && strlen(s) && commasCounter > 0)
        isValid = yieldError(illegalApearenceOfCommaBeforeFirstParameter);
    else if (!*s && strchr(s, ','))
        isValid = yieldError(wrongCommasSyntaxIllegalApearenceOfCommasInLine);

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

                isValid = yieldError(wrongCommasSyntaxExtra);
                commasCounter = 1;
            }
            else if (commasCounter < 1)
            {
                isValid = yieldError(wrongCommasSyntaxMissing);
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

    return isValid;
}

/* @ Function: countAndVerifyStringArguments
   @ Arguments: the function gets char * token which is the current token that we are about to parse the string argument from.
   @ Description: The function extracts the argument string of the .string instruction, than the function analyses\ parses the string.
   If the function encounter errors no opening or closing quotes, it yields (prints) the relevant error message.
   While the function parsing the arguments, ir also counts the length of the .string string (including the \0 at the end) that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .string instruction will take.
*/
Bool countAndVerifyStringArguments(char *line)
{
    char *s = 0, *args;
    Bool isValid = True;
    int size = 0;
    char *closing = 0, *opening = 0;
    line = strstr(line, STRING) + strlen(STRING);
    line = trimFromLeft(line);
    if (!line || !*line)
        return yieldError(emptyStringDeclatretion);
    args = strchr(line, '\"');
    if (args)
    {
        opening = strchr(line, '\"');
        closing = strrchr(line, '\"');
        if (opening == closing && (opening[0] != line[0]))
            isValid = yieldError(expectedQuotes);
        else if (opening == closing && (opening[0] == line[0]))
            yieldError(closingQuotesForStringIsMissing);
        s = opening;
        while (*s && *s != '\0')
        {
            s++;
            size++;
        }
    }
    else
    {
        yieldError(expectedQuotes);
        yieldError(closingQuotesForStringIsMissing);
        isValid = False;
    }
    if (isValid)
        increaseDataCounter((int)(size + 1)); /*counts the \0 at the end of the string as well*/

    return isValid;
}

/* @ Function: parseLine
   @ Arguments: The function gets char * token which is the current token that we are about to parse and char *line which is the current line being parsed
   @ Description: The function checks what is the current globalState, than checks what is the current token (an instruction, an operation, a label declaration...)
   The function extracts the argument string of the .string instruction, than the function analyses\ parses the string.
   If the function encounter errors no opening or closing quotes, it yields (prints) the relevant error message.
   While the function parsing the arguments, ir also counts the length of the .string string (including the \0 at the end) that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .string instruction will take.
*/
Bool parseLine(char *token, char *line)
{
    State (*globalState)() = &getGlobalState;
    Bool isValid = True;
    if (isComment(token))
        return True;

    if (isLabelDeclaration(token))
    {
        char labelName[MAX_LABEL_LEN] = {0};
        strcpy(labelName, token);
        printf("line 195 parse.c\n");
        if (!isLabelDeclarationStrict(token))
        {
            char *s;
            isValid = yieldError(missingSpaceBetweenLabelDeclaretionAndInstruction);
            token = strchr(token, ':');
            s = strchr(labelName, ':');
            *s = '\0';
            token++;
        }

        token = strstr(line, labelName) + strlen(labelName);

        if ((*globalState)() == firstRun)
            return handleLabel(labelName, token, line) && isValid;
        else
            return parseLine(token, line + strlen(token) + 1);
    }

    else if (isInstruction(token))
    {
        char *next;
        int type;
        Bool isValid = True;
        type = getInstructionType(token);
        next = (*globalState)() == firstRun ? strtok(NULL, " \t\n\f\r") : strtok(NULL, ", \t\n\f\r");

        if (next == NULL)
        {
            if (type == _TYPE_DATA || type == _TYPE_STRING)
                return type == _TYPE_DATA ? yieldWarning(emptyDataDeclaretion) : yieldWarning(emptyStringDeclatretion);
            else
                return type == _TYPE_ENTRY ? yieldWarning(emptyEntryDeclaretion) : yieldWarning(emptyExternalDeclaretion);
        }
        else
        {
            if ((*globalState)() == firstRun)
                return handleInstruction(type, token, next, line) && isValid;
            else
            {
                if (type == _TYPE_DATA)
                    return writeDataInstruction(next);
                else if (type == _TYPE_STRING)
                    return writeStringInstruction(next);
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
            yieldError(undefinedTokenNotOperationOrInstructionOrLabel);
        else
            yieldError(illegalApearenceOfCharacterInTheBegningOfTheLine);
    }

    return False;
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