#include "data.h"
extern Bool yieldError(Error err);
extern char *trimFromLeft(char *s);
extern State globalState;
extern unsigned currentLine;
extern const char *regs[REGS_SIZE];

int getNumberLength(int num)
{
    int length = 1;  /*counts the first digit of the given number*/
    while (num / 10) /*divides the number each time by 10 in order to remov digits its already counted*/
        length++;    /*increase the digit's counter by 1*/

    return length; /*returns the amount of digits in the given number*/
}
int countConsecutiveCommasAndTrimSpaces(char *s)
{
    int counter = 0;
    printf("before trim, s:%s ", s);
    s = trimFromLeft(s);
    printf("after trim, s:%s\n ", s);
    for (; s && *s == ','; counter++, s++)
        ;
    printf("before skipping commas, s:%s ", s);
    s = trimFromLeft(s);
    printf("after trim after skipping commas, s:%s\n", s);

    return counter;
}

int skipTokenNonDigitCharacters(char *s)
{
    int count = 0;
    for (; !isdigit(*s) && !isspace(*s) && *s != ','; s++, count++)
        ;
    return count;
}

Bool countAndVerifyDataArguments(char *line, char *token)
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
    len = strlen(p);
    memcpy(args, p, len);
    p = args;
    p = trimFromLeft(p);
    i = len - strlen(p);

    if (!isspace(*p))
        isValid = yieldError(missinSpaceAfterInstruction);

    if (*p == ',')
    {
        isValid = yieldError(illegalApearenceOfCommaBeforeFirstParameter);
        p++;
        i++;
    }

    while (p && i < len)
    {
        i = len - strlen(p);
        /*        printf("######\nargs[i]:%c\np:%c\nsize:%d commaCounter:%d num:%d c:%c\n\n######\n", args[i], *p, size, commasCounter, num, c); */
        commasCounter += countConsecutiveCommasAndTrimSpaces(&args[i]);
        /*         printf("After using  countConsecutiveCommasAndTrimSpaces(p)\nargs[i]:%c\np:%c\nsize:%d commaCounter:%d num:%d c:%c\n", args[i], *p, size, commasCounter, num, c);
         */
        if (!isdigit(p[0]) && !isspace(*p))
        {
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

                isValid = yieldError(illegalApearenceOfCharactersOnLine);
                i += skipTokenNonDigitCharacters(&args[i]);
            }
            if (*p == ',')
                commasCounter += countConsecutiveCommasAndTrimSpaces(&args[i]);
        }
        p = &args[i];
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

            sscanf(&args[i], "%d%c%n", &num, &c, &n);
            /*      printf("n:%d num:%d c:%c\n", n, num, c); */

            if (c && c == ',')
                commasCounter++;
            else if (c && c != ',' && !isspace(c) && c != '.')
                isValid = yieldError(illegalApearenceOfCharactersOnLine);

            else if (c == '.')
            {
                isValid = yieldError(wrongArgumentTypeNotAnInteger);
                p += n - getNumberLength(num);
                i += n - getNumberLength(num);
                sscanf(&args[i], "%d%n", &num, &n);
            }

            /*             else
                        {
                            size++;
                            printf("size %d\n", size);
                        } */
            size++;
            minusOrPlusFlag = False;
        }

        if (n)
        {
            p += n;
            /*
            i = i + n;
                p += n - getNumberLength(num);
             */
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
    /*     printf("inside handle State, token:%s\n", token); */

    switch (state)
    {
    case skipLine:
        return lineParsedSuccessfully;

    case newLine:
    {
        if (isComment(token))
            return lineParsedSuccessfully;

        if (isLabel(token))
            return skipToNextToken;

        else if (isInstruction(token))
        {
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

void parseSingleLine(char *line)
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
        state = handleState(token, line, state);
        switch (state)
        {
        case skipLine:
            state = lineParsedSuccessfully;
        case skipToNextToken:
        {

            if (globalState == firstRun)
                state = handleLabel(token, strtok(NULL, " \t \n"), line);
            else if (globalState == secondRun)
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

    if (!state)
        globalState = collectErrors;

    currentLine++;
}

Bool parseFile(FILE *fp, char *filename)
{
    int c = 0;
    int i = 0;
    char line[MAX_LINE_LEN + 1] = {0};
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

            parseSingleLine(line);
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

    return globalState != collectErrors ? True : False;
}
