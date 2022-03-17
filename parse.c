#include "data.h"
extern Bool yieldError(Error err);
extern char *trimFromLeft(char *s);
extern State globalState;
extern unsigned currentLine;
extern const char *regs[REGS_SIZE];

extern int countSpaceCharacters(char *s);

int countConsecutiveCommasAndTrimSpaces(char *s)
{
    int counter = 0;
    for (; s && *s == ','; counter++, s++)
        ;
    return counter;
}

int countLengthOfNonDigitToken(char *s)
{
    int count = 0;
    for (; !isdigit(*s) && *s != ','; s++, count++)
        ;
    return count;
}

Bool countAndVerifyDataArguments(char *line)
{
    /*
    token is only one  token at time connected to strtok, you can use it
    or you can use line argument which is the fullline without strtok,
    inside this function we create string variable named "args[MAX_LINE_LEN+1]"
    that takes the full string of arguments that starts straight after the .data
    token. choose what you want.
     */

    char args[MAX_LINE_LEN + 1] = {0};
    int size = 0, num = 0, n = 0, commasCounter = 0, i = 0, len = 0, skip = 0;
    char c = 0, *p = strstr(line, DATA) + strlen(DATA);
    Bool isValid = True;
    Bool minusOrPlusFlag = False;

    len = strlen(p);
    memcpy(args, p, len);
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
                        minusOrPlusFlag = True;
                    else
                    {
                        isValid = yieldError(illegalApearenceOfCharactersOnLine);
                        minusOrPlusFlag = False;
                    }
                    skip = 1;
                }
                else if (*p == ',')
                {
                    skip = countConsecutiveCommasAndTrimSpaces(p);
                    commasCounter += skip;
                }
                else
                {
                    isValid = yieldError(illegalApearenceOfCharactersOnLine);
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
                    isValid = yieldError(illegalApearenceOfCharactersOnLine);

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

    printf("line 189 commaCounter:%d size:%d\n", commasCounter, size);
    if (commasCounter > (size - 1))
        isValid = yieldError(illegalApearenceOfCommaAfterLastParameter);

    printf("isValid:%d\n", isValid);
    if (isValid)
        increaseDataCounter(size);

    return isValid;
}

Bool countAndVerifyStringArguments(char *token)
{

    if (isInstruction(token))
        token = strtok(NULL, " \t \n");
    if (token[0] == '\"' && token[strlen(token) - 1] != '\"')
        return yieldError(closingQuotesForStringIsMissing);
    else if (token[0] != '\"')
        return yieldError(expectedQuotes);

    increaseDataCounter((int)(strlen(token) - 1)); /*counts the \0 at the end of the string as well*/

    return True;
}

ParseState handleState(char *token, char *line, ParseState state)

{
    /*     printf("inside handleState, token:%s line:%s\n", token, line);
     */
    switch (state)
    {
    case skipLine:
        return lineParsedSuccessfully;

    case newLine:
    {
        if (isComment(token))
            return lineParsedSuccessfully;

        if (isLabel(token))
        {
            if (globalState == firstRun)
            {
                return handleLabel(token, strtok(NULL, " \t \n"), line) ? lineParsedSuccessfully : Err;
            }
            else
                return skipToNextToken;
        }

        else if (isInstruction(token))
        {
            /*           printf("line 205, is instruction\n"); */
            if (globalState == firstRun)
                return handleInstruction(getInstructionType(token), token, strtok(NULL, " \t \n"), line);
            else
            {
                int type = getInstructionType(token);
                token = strtok(NULL, ", \t \n");

                if (type == _TYPE_DATA)
                {
                    return writeDataInstruction(token) ? lineParsedSuccessfully : Err;
                }
                else if (type == _TYPE_STRING)
                {
                    return writeStringInstruction(token) ? lineParsedSuccessfully : Err;
                }
                else
                    return lineParsedSuccessfully;
            }
        }

        else if (isOperation(token))
        {
            char args[MAX_LINE_LEN] = {0};
            /*            line = line + strlen(token); */
            strcpy(args, (line + strlen(token)));
            return globalState == firstRun ? handleOperation(token, args) : writeOperationBinary(token, args) ? lineParsedSuccessfully
                                                                                                              : Err;
        }
        else
        {
            if (globalState == firstRun)
            {
                yieldError(illegalLabelUseExpectedOperationOrInstruction);
                return Err;
            }
        }
    }

    default:
        break;
    }

    return state;
}

Bool parseSingleLine(char *line)
{
    ParseState state = newLine;
    char lineCopy[MAX_LINE_LEN] = {0};
    char *token;
    printf("\ninside parseSingleLine, Line Number (%d):\n%s\n", currentLine, line);
    memcpy(lineCopy, line, strlen(line));
    if (globalState == firstRun)
        token = strtok(lineCopy, " \t \n");
    else if (globalState == secondRun)
        token = strtok(lineCopy, ", \t \n");

    while (token != NULL && state != lineParsedSuccessfully)
    {
        /*         printf("inside parseSingleLine while loop, token:%s\n", token); */

        state = handleState(token, line, state);
        switch (state)
        {
        case skipLine:
            state = lineParsedSuccessfully;
        case skipToNextToken:
        {
            line = line + strlen(token) + 1;
            state = handleState(strtok(NULL, ", \t \n"), line, newLine);
            break;
        }

        case Err:
        {
            break;
        }
        default:
            break;
        }

        if (globalState == firstRun)
            token = strtok(NULL, " \t \n");
        else if (globalState == secondRun)
            token = strtok(NULL, ", \t \n");
    }

    currentLine++;

    printf("state:%d\n", state);
    return state ? True : False;
}

Bool parseFile(FILE *fp, char *filename)
{
    int c = 0;
    int i = 0;
    char line[MAX_LINE_LEN + 1] = {0};
    Bool isValidCode = True;

    currentLine = 1;
    if (globalState == secondRun)
        printf("\n\n\nSecond Run:\n");
    else
        printf("\n\n\nFirst Run:\n");

    while (((c = fgetc(fp)) != EOF))
    {
        if (globalState != secondRun && (i >= MAX_LINE_LEN - 1 && c != '\n'))
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

        else if (isspace(c))
            line[i++] = ' ';

        else
        {
            if (isprint(c))
                line[i++] = c;
        }
    }

    if (i > 0)
    {
        if (!parseSingleLine(line))
            isValidCode = False;

        memset(line, 0, i);
    }
    if (!isValidCode)
        globalState = collectErrors;

    return globalState != collectErrors ? True : False;
}
