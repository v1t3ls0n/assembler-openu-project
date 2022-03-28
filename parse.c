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

extern char *splitToken(char *s);

/* @ Function: countAndVerifyDataArguments
   @ Arguments: the function get char * line which is the current line that we are about to parse the data arguments from.
   @ Description: The function extracts the argument string of the .data instruction, than the function analyses\ parses the string.
   If the function encounter errors, an illegal comma or illegal arguments, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
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

    /*checks if ther is an illegal appearence of comma, a missing comma or an extra comma. If there is it changes isValid to be false and yeild errors*/
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

<<<<<<< HEAD
/* @ Function: countAndVerifyStringArguments
   @ Arguments: the function gets char * token which is the current token that we are about to parse the string argument from.
   @ Description: The function extracts the argument string of the .string instruction, than the function analyses\ parses the string.
   If the function encounter errors- no opening or closing  (or no opening and closing) quotes, it yields (prints) the relevant error message and keeps on parsing the rest of the line in order to find and report all the errors.
   While the function parsing the arguments, ir also counts the length of the .string string (including the \0 at the end) that will take size in the data memory.
   In the end of the function, if after parsing the line turns out to be valid, it increases the data counter with the size in memory that the current .string instruction will take.
*/
=======
>>>>>>> 6bed85ffa8ad18bbd86395e1f0d312d190915fe3
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

<<<<<<< HEAD
/* @ Function: parseLine
   @ Arguments: The function gets char * token which is the first token of the line that we are about to parse and char *line which is the current line being parsed
   @ Description: The function checks what is the current globalState, than checks what is the first token (an instruction, an operation, a label declaration...)
    If the globalState is equal to firstRun, according to the first token, it finds out what kind of line is the current line and calls other function so they can handle the line.
    If the globalState is equal to secondRun, according to the first token, it calls function to write the compiled memory.
    If it finds some errors, it yields (prints) the relevant error message.
    If the line turns out to be valid, it returns true, else it returns false.
*/
=======
>>>>>>> 6bed85ffa8ad18bbd86395e1f0d312d190915fe3
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
/* @ Function:handleSingleLine
   @ Arguments: This function gets char *line which is the current line that is being parsed.
   @ Description: The function checks what is the current globalState, and by that decides how to split the line into tokens (what is the delimeter).
   Than the function calls the function parseLine in order to check if there are errors in the line.
   Than its increases by 1 the number of line it parses (for the next line to come).
   Returns true if the line is valid and false if it isn't.
*/
Bool handleSingleLine(char *line)
{
    State (*globalState)() = &getGlobalState;
    char lineCopy[MAX_LINE_LEN] = {0};
    Bool result = True;
    char *token;
    strcpy(lineCopy, line);
    token = ((*globalState)() == firstRun) ? strtok(lineCopy, " \t\n\f\r") : strtok(lineCopy, ", \t\n\f\r");
    result = parseLine(token, line); /*checks if there are errors in the line, result would be equal to false if there are*/
    (*currentLineNumberPlusPlus)();
    return result;
}
<<<<<<< HEAD
/* @ Function:parseAssemblyCode
   @ Arguments: The function gets FILE *src which is a file after the pre proccesoring.
   @ Description: The function parses the file, splits it to tokens and calls the right functions to complete the first and second run.
*/
=======
>>>>>>> 6bed85ffa8ad18bbd86395e1f0d312d190915fe3
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