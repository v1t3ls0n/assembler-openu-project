/*



Bool countAndVerifyDataArgumentsV2(char *line)
{

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

if (commasCounter > (size - 1))
    isValid = yieldError(illegalApearenceOfCommaAfterLastParameter);

if (isValid)
    increaseDataCounter(size);

return isValid;
}

int num = 0;
char *p = s;
p++;
num = atoi(p);

printf("inside getRegisteryNumber, number:%d\n", num);
if (operand[0] == 'r' && isdigit(operand[strlen(operand) - 1]))
    yieldError(wrongRegisteryReferenceUndefinedReg);

int parseAllStrict(FILE *target)
{
    ParseState state = evalToken;
    Bool isMacro = False;
    int c = 0, i = 0, j = 0;
    int start = 1, end = 1, current = 1;
    char token[MAX_LINE_LEN] = {0};
    char macroName[MAX_LABEL_LEN] = {0};
    while ((c = fgetc(target)) != EOF)
    {
        current++;
        if (c == '\n' && state == skipLine)
        {
            if (state == skipLine)
                state = evalToken;

            current++;
            currentLineNumber++;
        }

        if (state != skipLine)
        {

            if (!isspace(c) && j < MAX_LABEL_LEN && i < MAX_LABEL_LEN)
            {
                if (state == getMacroName)
                    macroName[i++] = c;

                else
                    token[j++] = c;
            }
            else if (isspace(c) && (j > 0 || i > 0))
            {

                if (i > 0)
                    macroName[i] = '\0';
                if (j > 0)
                    token[j] = '\0';

                if (state == evalToken)
                {
                    if (!isMacro && isMacroOpening(token))
                    {
                        state = getMacroName;
                        isMacro = True;
                    }
                    if (!isMacro && isMacroClosing(token))
                    {
                        yieldError(macroClosingWithoutAnyOpenedMacro);
                        state = skipLine;
                    }

                    else if (isMacro)
                    {

                        if (isMacroClosing(token))
                        {
                            end = current;

                            updateMacro(macroName, start, end);
                            memset(macroName, 0, i);
                            i = 0;
                            isMacro = False;
                            state = skipLine;
                        }

                        else if (isMacroOpening(token))
                        {
                            if ((currentLineNumber - start) < 2)
                            {
                                yieldError(useOfNestedMacrosIsIllegal);
                            }
                            updateMacro(macroName, -1, -1);
                            memset(macroName, 0, i);
                            i = 0;
                            isMacro = False;
                            state = skipLine;
                        }
                    }

                    memset(token, 0, j);
                    j = 0;
                }

                else if (state == getMacroName)
                {

                    if (isLegalMacroName(macroName))
                    {
                        start = current;
                        addMacro(macroName, start, -1);
                        i = 0;
                        state = evalToken;
                    }
                    else
                    {
                        yieldError(illegalMacroNameUseOfSavedKeywords);
                        memset(macroName, 0, i);
                        i = 0;
                        state = skipLine;
                        isMacro = False;
                    }
                }
            }
        }
    }

    return 1;
}

FILE *createCopyFromSourceFile(FILE *source, char *fileName)
{
    FILE *target;
    int c = 0;
    fileName[strlen(fileName) - 1] = 'm';
    target = fopen(fileName, "w+");
    if (target == NULL)
    {
        fclose(source);
        printf("Press any key to exit...\n");
        exit(1);
    }
    while ((c = fgetc(source)) != EOF)
    {

        fputc(c, target);
    }

    printf("File copied successfully.\n");
    fclose(source);

    return target;
}
* /