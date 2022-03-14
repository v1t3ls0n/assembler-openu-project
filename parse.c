#include "data.h"
extern Bool yieldError(Error err);

/*
1. before entering while loop on tokens, if token[0]==','
yield error about illegal comma before first data argument,
move the counter ++ farward and set isValid to false

2.next part in token should be integer/+/-,
if +/- token++, if after one +/- first character is not a
decimal number yield error, move to next token with strtok
or strchr.

 */

int getTokenLength(char *s)
{
    int i = 0;
    while (isspace(*s))
    {
        s++;
        i++;
    }

    return i;
}

char *leftTrim(char *s)
{
    while (isspace(*s))
        s++;
    return s;
}
char *rightTrim(char *s)
{
    char *end = s + strlen(s);
    while (isspace(*--end))
        ;
    *(end + 1) = '\0';
    return s;
}

char *trimString(char *s)
{
    return rightTrim(leftTrim(s));
}
Bool countAndVerifyDataArguments(char *line)
{
    char args[MAX_LINE_LEN + 1] = {0};

    int size = 0, num = 0, n = 0, commasCounter = 0, i = 0, len = 0;
    char c = 0;

    char *p = strstr(line, DATA) + strlen(DATA);
    Bool isValid = True;
    Bool minusOrPlusFlag = False;
    Bool isNum = False;
    len = strlen(p);
    memcpy(args, p, len);
    args[len] == '\0';
    p = args;
    p = leftTrim(p);
    if (p[0] == ',')
        isValid = yieldError(wrongInstructionSyntaxIllegalCommaPosition);

    while (i < len)
    {

        p = leftTrim(p);
        i = len - strlen(p);
        while (*p == ',')
        {
            commasCounter++;
            p++;
            i++;
        }
        while (*p == '-' || *p == '+')
        {
            if (!minusOrPlusFlag)
                minusOrPlusFlag = True;
            else
                isValid = yieldError(illegalApearenceOfCharactersOnLine);
            p++;
            i++;
        }

        if (isprint(*p) && !isdigit(p[0]) && !isspace(*p))
        {
            isValid = yieldError(illegalApearenceOfCharactersOnLine);
            while (!isdigit(*p) && *p != ',')
            {
                p++;
                i++;
            }
        }

        else if (isdigit(*p))
        {
            printf("size:%d\n", size);
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
            else if (commasCounter > 1)
            {
                isValid = yieldError(wrongInstructionSyntaxExtraCommas);
                commasCounter = 0;
            }
            else if (commasCounter < 1 && size > 1)
                isValid = yieldError(wrongInstructionSyntaxMissinCommas);

            else if (commasCounter == 1 || (commasCounter == 0 && size == 0))
                size++;

            commasCounter = 0;

            if (c == ',')
                commasCounter++;

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

    if (commasCounter > 0)
        isValid = yieldError(wrongInstructionSyntaxIllegalCommaPosition);
    if (isValid)
        increaseDataCounter(size);

    return isValid;
}
