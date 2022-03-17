#include "data.h"
extern Bool yieldError(Error err);
extern char *trimFromLeft(char *s);
extern State globalState;
extern unsigned currentLine;
extern const char *regs[REGS_SIZE];

extern int countSpaceCharacters(char *s);
int getNumberLength(int num)
{
    int length = 1;
    /*counts the first digit of the given number*/
    while (num / 10) /*divides the number each time by 10 in order to remov digits its already counted*/
        length++;    /*increase the digit's counter by 1*/

    return length; /*returns the amount of digits in the given number*/
}

int countConsecutiveCommasAndTrimSpaces(char *s)
{
    int counter = 0;
    memcpy(s, trimFromLeft(s), strlen(s));
    for (; s && *s == ','; counter++, s++)
        ;
    memcpy(s, trimFromLeft(s), strlen(s));
    return counter;
}

int skipTokenNonDigitCharacters(char *s)
{
    int count = 0;
    for (; (!isdigit(*s) || isspace(*s)) && *s != ','; s++, count++)
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
    int size = 0, num = 0, n = 0, commasCounter = 0, i = 0, len = 0;
    char c = 0, *p = strstr(line, DATA) + strlen(DATA);
    Bool isValid = True;
    Bool minusOrPlusFlag = False;
    printf("line 52\n");

    len = strlen(p);
    memcpy(args, p, len);
    p = args;
    p = trimFromLeft(p);
    i = len - strlen(p);
    printf("line 57, args: %s\n", args);
    if (*p == ',')
    {
        isValid = yieldError(illegalApearenceOfCommaBeforeFirstParameter);
        p++;
        i++;
    }

    while (p && i < len)
    {
        i = len - strlen(p);
        commasCounter += countConsecutiveCommasAndTrimSpaces(p);

        /*         printf("After using  countConsecutiveCommasAndTrimSpaces(p)\nargs[i]:%c\np:%c\nsize:%d commaCounter:%d num:%d c:%c\n\n\n", args[i], *p, size, commasCounter, num, c);
         */
        printf("line 72, p[0] : %c args[i] : %c\n", p[0], args[i]);
        if (!isdigit(p[0]) && !isspace(*p))
        {
            printf("line 74\n");
            if (*p == '-' || *p == '+')
            {

                if (!minusOrPlusFlag)
                    minusOrPlusFlag = True;
                else
                    isValid = yieldError(illegalApearenceOfCharactersOnLine);
                p++;
                i++;
            }
            else if (*p != ',')
            {
                int charCount = skipTokenNonDigitCharacters(p);
                isValid = yieldError(illegalApearenceOfCharactersOnLine);
                i += charCount;
                p += charCount;
                size++;
            }
            if (*p == ',')
                commasCounter += countConsecutiveCommasAndTrimSpaces(p);
        }
        if (commasCounter < 1 && size > 0)
        {
            isValid = yieldError(wrongInstructionSyntaxMissinCommas);
            commasCounter = size;
        }

        else if (commasCounter > 0 && (size < (commasCounter - 1)))
        {
            isValid = yieldError(wrongInstructionSyntaxExtraCommas);
            commasCounter = size;
        }
        else if (size > commasCounter)
        {
            isValid = yieldError(wrongInstructionSyntaxMissinCommas);
            commasCounter = size;
        }

        if (isdigit(*p))
        {
            i = len - strlen(p);
            sscanf(&args[i], "%d%n%c", &num, &n, &c);

            printf("line 117, num: %d c: %c n: %d args[i] : %c p: %s\n", num, c, n, args[i], p);

            if (c && c == ',')
                commasCounter++;
            else if (c && c != ',' && !isspace(c) && c != '.')
                isValid = yieldError(illegalApearenceOfCharactersOnLine);

            else if (c == '.')
            {
                /*isValid = yieldError(wrongArgumentTypeNotAnInteger);*/
                /*      p += n - getNumberLength(num);
                     i += n - getNumberLength(num); */
                p += n + 1;
                i += n + 1;
                printf("line 126, num: %d c: %c n: %d args[i] : %c p: %s\n", num, c, n, args[i], p);
                isValid = yieldError(wrongArgumentTypeNotAnInteger);
                sscanf(&args[i], "%d%n", &num, &n);
                printf("line 138, num: %d n: %d", num, n);
            }
            size++;
            minusOrPlusFlag = False;
        }
        if (n)
        {
            p += n;
            i += n;
            c = n = num = 0;
        }
        else
        {
            p++;
            i++;
        }
    }

    if (commasCounter > size)
        isValid = yieldError(illegalApearenceOfCommaAfterLastParameter);

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
    printf("inside handleState, token:%s line:%s\n", token, line);
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
            printf("line 205, is instruction\n");
            if (globalState == firstRun)
                return handleInstruction(getInstructionType(token), token, strtok(NULL, " \t \n"), line) ? lineParsedSuccessfully : Err;
            else
            {
                int type = getInstructionType(token);
                token = strtok(NULL, ", \t \n");

                if (type == _TYPE_DATA)
                {
                    return writeDataInstruction(token);
                }
                else if (type == _TYPE_STRING)
                {
                    return writeStringInstruction(token);
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
            return globalState == firstRun ? handleOperation(token, args) : writeOperationBinary(token, args);
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

    return lineParsedSuccessfully;
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
        printf("inside parseSingleLine while loop, token:%s\n", token);

        state = handleState(token, line, state);
        switch (state)
        {
        case skipLine:
            state = lineParsedSuccessfully;
        case skipToNextToken:
        {
            printf("inside skip to next token\n");

            if (globalState == secondRun)
            {
                line = line + strlen(token) + 1;
                state = handleState(strtok(NULL, ", \t \n"), line, newLine);
            }

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
    return state;
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

            globalState = collectErrors;
            yieldError(maxLineLengthExceeded);
            memset(line, 0, MAX_LINE_LEN);
            i = 0;
        }

        if (c == '\n')
        {

            isValidCode = isValidCode && parseSingleLine(line);
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
        parseSingleLine(line);
        memset(line, 0, i);
    }
    if (!isValidCode)
        globalState = collectErrors;

    return globalState != collectErrors ? True : False;
}
