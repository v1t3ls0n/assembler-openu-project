#include "data.h"
extern Bool yieldError(Error err);
extern Bool yieldWarning(Warning err);
extern char *trimFromLeft(char *s);
extern int countConsecutiveCommas(char *s);
extern int countLengthOfNonDigitToken(char *s);
static void (*setCurrentLineToStart)() = &resetCurrentLineNumber;
/* static void (*setFileName)(char *) = &setCurrentFileName; */
static void (*currentLineNumberPlusPlus)() = &increaseCurrentLineNumber;

/* extern State getGlobalState();
 */

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
    Bool minusOrPlusFlag = False;
    char args[MAX_LINE_LEN + 1] = {0};
    int size = 0, num = 0, n = 0, commasCounter = 0, i = 0, len = 0, skip = 0;
    char c = 0, *p;
    /* we make the pointer p to point on the position of the first character coming sfter the .data
     instruction within the full line, so that p will point on the begining of the arguments string*/
    p = (strstr(line, DATA) + strlen(DATA));
    len = strlen(p);
    /*copies the string of arguments pointer by p into the args local string we will use for parsing*/
    memcpy(args, p, len);
    if (!strlen(args))
        return yieldWarning(emptyDataDeclaretion);
    p = args;
    p = trimFromLeft(p);
    i = len - strlen(p);

    if (*p == ',')
    {
        isValid = yieldError(illegalApearenceOfCommaBeforeFirstParameter);
        p++;
        i++;
    }

    while (p && i < len)
    {

        if (!isspace(p[0]))
        {
            i = len - strlen(p);
            if (!isdigit(p[0]))
            {

                if (*p == '-' || *p == '+')
                {
                    if (!minusOrPlusFlag)
                    {
                        if (isspace(p[1]))
                        {
                            isValid = yieldError(afterPlusOrMinusSignThereMustBeANumber);
                            minusOrPlusFlag = False;
                        }
                        else
                            minusOrPlusFlag = True;
                    }
                    else
                    {
                        isValid = yieldError(expectedNumber);
                        minusOrPlusFlag = False;
                    }
                    skip = 1;
                }
                else if (*p == ',')
                {
                    skip = countConsecutiveCommas(p);
                    commasCounter += skip;
                }
                else
                {
                    isValid = yieldError(expectedNumber);
                    skip = countLengthOfNonDigitToken(p);
                    size++;
                }
            }
            else
            {
                if (commasCounter < 1 && size > 0)
                {
                    isValid = yieldError(wrongInstructionSyntaxMissinCommas);
                    commasCounter = size;
                }

                else if (commasCounter > size)
                {
                    isValid = yieldError(wrongInstructionSyntaxExtraCommas);
                    commasCounter = size;
                }
                else if (size > commasCounter)
                {
                    isValid = yieldError(wrongInstructionSyntaxMissinCommas);
                    commasCounter = size;
                }

                i = len - strlen(p);
                sscanf(&args[i], "%d%n%c", &num, &n, &c);
                if (c && c != ',' && !isspace(c) && c != '.')
                {
                    isValid = yieldError(illegalApearenceOfCharactersOnLine);
                    n++;
                    p++;
                    i++;
                }

                else if (c == '.')
                {
                    isValid = yieldError(wrongArgumentTypeNotAnInteger);
                    p += n + 1;
                    i += n + 1;
                    sscanf(&args[i], "%d%n", &num, &n);
                }
                size++;
                minusOrPlusFlag = False;
                skip = n > 0 ? n : 1;
                c = n = num = 0;
            }
        }
        else
            skip = 1;

        p += skip;
        i += skip;
    }

    /*     printf("line 189 commaCounter:%d size:%d\n", commasCounter, size); */
    if (commasCounter > (size - 1))
        isValid = yieldError(illegalApearenceOfCommaAfterLastParameter);

    /*     printf("isValid:%d\n", isValid); */
    if (isValid)
        increaseDataCounter(size);

    return isValid;
}

Bool countAndVerifyStringArguments(char *token)
{

    if (isInstruction(token))
        token = strtok(NULL, " \t \n");

    if (token)
    {
        if (token[0] == '\"' && (token[strlen(token) - 1] != '\"' || strlen(token) < 2))
            return yieldError(closingQuotesForStringIsMissing);
        else if (token[0] != '\"')
            return yieldError(expectedQuotes);

        increaseDataCounter((int)(strlen(token) - 1)); /*counts the \0 at the end of the string as well*/
    }
    else
        return yieldWarning(emptyStringDeclatretion);

    return True;
}

ParseState handleState(char *token, char *line)
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
            char *next = (*globalState)() == firstRun ? strtok(NULL, " \t \n") : strtok(NULL, ", \t \n");
            if (!next)
                return yieldError(emptyLabelDecleration);

            if ((*globalState)() == firstRun)
                return handleLabel(token, next, line) ? lineParsedSuccessfully : Err;
            else
                return handleState(next, line + strlen(token) + 1);
        }
    }

    else if (isInstruction(token))
    {

        char *next = (*globalState)() == firstRun ? strtok(NULL, " \t \n") : strtok(NULL, ", \t \n");
        int type = getInstructionType(token);
        if (!next)
        {
            if (type == _TYPE_DATA || type == _TYPE_STRING)
                return type == _TYPE_DATA ? yieldWarning(emptyDataDeclaretion) : yieldWarning(emptyStringDeclatretion);
            else
                return type == _TYPE_ENTRY ? yieldWarning(emptyEntryDeclaretion) : yieldWarning(emptyExternalDeclaretion);
        }
        else
        {
            if ((*globalState)() == firstRun)
                return handleInstruction(type, token, next, line);
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

Bool parseSingleLine(char *line)
{
    State (*globalState)() = &getGlobalState;
    ParseState state = newLine;
    char lineCopy[MAX_LINE_LEN] = {0};
    char *token;
    /*     printf("inside parse single line, line:%s\n", line);
     */
    memcpy(lineCopy, line, strlen(line));
    token = (*globalState)() == firstRun ? strtok(lineCopy, " \t \n") : strtok(lineCopy, ", \t \n");
    state = handleState(token, line);
    (*currentLineNumberPlusPlus)();

    return state == lineParsedSuccessfully
               ? True
               : False;
}

void parseAssemblyCode(FILE *fp, char *filename)
{
    State (*globalState)() = &getGlobalState;
    int c = 0;
    int i = 0;
    char line[MAX_LINE_LEN + 1] = {0};
    Bool isValidCode = True;
    State nextState = (*globalState)() == firstRun ? secondRun : exportFiles;
    /*     (*setFileName)(filename);
     */
    (*setCurrentLineToStart)();
    if ((*globalState)() == secondRun)
        printf("\n\n\nSecond Run:\n");
    else
        printf("\n\n\nFirst Run:\n");

    while (((c = fgetc(fp)) != EOF))
    {
        if ((*globalState)() != secondRun && (i >= MAX_LINE_LEN - 1 && c != '\n'))
        {

            isValidCode = False;
            yieldError(maxLineLengthExceeded);
            memset(line, 0, MAX_LINE_LEN);
            i = 0;
        }

        if (c == '\n' && i > 0)
        {
            if (!parseSingleLine(line))
                isValidCode = False;

            memset(line, 0, MAX_LINE_LEN);
            i = 0;
        }

        else if (isspace(c) && i > 0)
        {
            line[i++] = ' ';
        }
        else
        {
            if (isprint(c) && !isspace(c))
                line[i++] = c;
        }
    }

    if (i > 0)
    {
        if (!parseSingleLine(line))
            isValidCode = False;
    }

    if (!isValidCode)
        nextState = collectErrors;

    updateGlobalState(nextState);
}