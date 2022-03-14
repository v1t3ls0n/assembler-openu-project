#include "data.h"
extern Bool yieldError(Error err);
char *trimFromLeft(char *s);

Bool countAndVerifyDataArguments(char *line)
{
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

    if (*p == ',')
        isValid = yieldError(wrongInstructionSyntaxIllegalCommaPosition);

    while (p && i < len)
    {
        p = trimFromLeft(p);
        i = len - strlen(p);
        while (*p == ',')
        {
            commasCounter++;
            p++;
        }
        while (*p == '-' || *p == '+')
        {
            if (!minusOrPlusFlag)
                minusOrPlusFlag = True;
            else
                isValid = yieldError(illegalApearenceOfCharactersOnLine);
            p++;
        }

        if (isprint(*p) && !isdigit(p[0]) && !isspace(*p))
        {
            isValid = yieldError(illegalApearenceOfCharactersOnLine);
            while (!isdigit(*p) && *p != ',')
                p++;
        }

        if (commasCounter < 1 && size > 1)
            isValid = yieldError(wrongInstructionSyntaxMissinCommas);

        else if (commasCounter > 1)
            isValid = yieldError(wrongInstructionSyntaxExtraCommas);

        if (isdigit(*p))
        {
            i = len - strlen(p);
            sscanf(&args[i], "%d%c%n", &num, &c, &n);

            if (c && c != ',' && !isspace(c) && c != '.')
                isValid = yieldError(expectedNumber);
            else if (c == '.')
            {
                isValid = yieldError(wrongArgumentTypeNotAnInteger);
                p += n;
                i += n;
                sscanf(&args[i], "%d%n", &num, &n);
            }

            else if (commasCounter == 1 || (commasCounter == 0 && size == 0))
                size++;

            commasCounter = 0;
            minusOrPlusFlag = False;
        }

        if (c == ',')
            commasCounter++;

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

    if (commasCounter > 0)
        isValid = yieldError(wrongInstructionSyntaxIllegalCommaPosition);
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

char *trimFromLeft(char *s)
{
    while (isspace(*s))
        s++;
    return s;
}