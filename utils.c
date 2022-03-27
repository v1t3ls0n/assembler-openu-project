#include "data.h"
const char *regs[REGS_SIZE] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10",
                               "r11", "r12", "r13", "r14", "r15"};
Bool isMacroOpening(char *s)
{
    return !strcmp(s, "macro") ? True : False;
}

Bool isMacroClosing(char *s)
{
    return !strcmp(s, "endm") ? True : False;
}
Bool isPossiblyUseOfMacro(char *s)
{
    return !isLabelDeclaration(s) && !isOperation(s) && !isComment(s) && !isInstructionStrict(s) && !isMacroClosing(s) && !isMacroOpening(s);
}

Bool isLegalMacroName(char *s)
{
    return !isInstructionStrict(s) && !isOperation(s) ? True : False;
}

Bool isInstruction(char *s)
{

    if ((!strcmp(s, DATA) || !strcmp(s, STRING) || !strcmp(s, ENTRY) || !strcmp(s, EXTERNAL)))
        return True;

    else if (strstr(s, DATA) != NULL || strstr(s, STRING) != NULL || strstr(s, ENTRY) != NULL || strstr(s, EXTERNAL) != NULL)
    {
        /*         yieldError(missinSpaceAfterInstruction); */
        return True;
    }
    else
        return False;
}

Bool isInstructionStrict(char *s)
{

    return ((!strcmp(s, DATA) || !strcmp(s, STRING) || !strcmp(s, ENTRY) || !strcmp(s, EXTERNAL))) ? True : False;
}

Bool isRegistery(char *s)
{
    int len = strlen(s);
    int i = 0;
    if (s[0] == 'r' && len >= 2)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(regs[i], s) == 0))
                return True;
            i++;
        }
    }
    return False;
}
Bool isValidImmediateParamter(char *s)
{
    int i, len = strlen(s);
    if (len < 2 || s[0] != '#' || (!(s[1] == '-' || s[1] == '+' || isdigit(s[1]))))
        return False;
    for (i = 2; i < len; i++)
        if (!isdigit(s[i]))
            return False;
    return True;
}
Bool isValidIndexParameter(char *s)
{
    int len = strlen(s);
    if (len < 6)
        return False;

    else if (!(s[len - 1] == ']' && s[len - 4] == 'r' && s[len - 5] == '['))
        return False;
    else
    {
        s = strchr(s, '[');
        s++;

        s[strlen(s) - 1] = 0;

        if (isRegistery(s) && getRegisteryNumber(s) < 10)
            return False;
    }
    return True;
}

Bool isComment(char *s)
{
    s = trimFromLeft(s);
    return s[0] == ';' ? True : False;
}
Bool isOperation(char *s)
{

    return (getOperationByName(s) != NULL) ? True : False;
}

Bool isLabelDeclarationStrict(char *s)
{
    return s[strlen(s) - 1] == ':' ? True : False;
}

Bool isLabelDeclaration(char *s)
{
    return strchr(s, ':') != NULL ? True : False;
}

int getInstructionType(char *s)
{
    if (strstr(s, DATA) != NULL)
        return _TYPE_DATA;
    if (strstr(s, STRING) != NULL)
        return _TYPE_STRING;
    if (strstr(s, ENTRY) != NULL)
        return _TYPE_ENTRY;
    if (strstr(s, EXTERNAL) != NULL)
        return _TYPE_EXTERNAL;
    return False;
}

int getRegisteryNumber(char *s)
{
    s++;
    return atoi(s);
}

char *getInstructionNameByType(int type)
{
    switch (type)
    {
    case _TYPE_DATA:
        return strcat(DATA, "\0");

    case _TYPE_STRING:
        return strcat(STRING, "\0");

    case _TYPE_ENTRY:
        return strcat(ENTRY, "\0");

    case _TYPE_EXTERNAL:
        return strcat(EXTERNAL, "\0");

    default:
        break;
    }

    return NULL;
}

char *getInstructionName(char *s)
{
    if (strstr(s, DATA) != NULL)
        return DATA;
    if (strstr(s, STRING) != NULL)
        return STRING;
    if (strstr(s, ENTRY) != NULL)
        return ENTRY;
    if (strstr(s, EXTERNAL) != NULL)
        return EXTERNAL;

    return 0;
}

Bool verifyLabelNaming(char *s)
{
    int i = 0;
    int labelLength = strlen(s);

    /* if label name does not start with a alphabet letter */
    if (isalpha(s[0]) == 0)
        return False;

    /* maximum label name length is 31 characters */
    if (strlen(s) > MAX_LABEL_LEN)
        return False;

    if (isRegistery(s))
        return False;

    else if (isOperationName(s))
        return False;
    else
    {

        while (i < labelLength)
        {
            if (!isalnum(s[i]))
                return False;

            i++;
        }
    }

    return True;
}

Bool verifyLabelNamingAndPrintErrors(char *s)
{
    int i = 0;
    int labelLength = strlen(s);

    /* if label name does not start with a alphabet letter */
    if (isalpha(s[0]) == 0)
        return yieldError(illegalLabelNameUseOfCharacters);

    /* maximum label name length is 31 characters */
    else if (strlen(s) > MAX_LABEL_LEN)
        return yieldError(illegalLabelNameLength);

    else if (isRegistery(s))
        return yieldError(illegalLabelNameUseOfSavedKeywordUsingRegisteryName);

    else if (isOperationName(s))
        return yieldError(illegalLabelNameUseOfSavedKeywordUsingOperationName);

    else
    {

        while (i < labelLength)
        {
            if (!isalnum(s[i]))
                return yieldError(illegalLabelNameUseOfCharacters);

            i++;
        }
    }

    return True;
}
