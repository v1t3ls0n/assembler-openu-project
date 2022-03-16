#include "data.h"
extern HexWord *convertNumToHexWord(int num);
extern const char *regs[REGS_SIZE];
extern Operation operations[OP_SIZE];
char *trimFromLeft(char *s)
{
    while (isspace(*s))
        s++;
    return s;
}
char *decToHex(int num)
{
    int i = num, size = 0;
    char *hex;
    for (size = 0; i > 0; i = i / 16)
        size++;
    hex = (char *)calloc(size, sizeof(char));
    sprintf(hex, "%05x", num);
    return hex;
}

char *numToBin(int num)
{
    int i = 0;
    unsigned int result;
    char *word = (char *)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
    char hex[6];

    if (num < 0)
    {
        result = abs(num);
        result = ~result;
        result++;
        sprintf(hex, "%05x", (int)(result & 0x4ffff));
    }
    else
        sprintf(hex, "%05x", (int)num & 0xfffff);

    while (hex[i] != '\0')
    {
        switch (hex[i])
        {

        case '0':
            strcat(word, "0000");
            break;
        case '1':
            strcat(word, "0001");
            break;
        case '2':
            strcat(word, "0010");
            break;
        case '3':
            strcat(word, "0011");
            break;
        case '4':
            strcat(word, "0100");
            break;
        case '5':
            strcat(word, "0101");
            break;
        case '6':
            strcat(word, "0110");
            break;
        case '7':
            strcat(word, "0111");
            break;
        case '8':
            strcat(word, "1000");
            break;
        case '9':
            strcat(word, "1001");
            break;
        case 'A':
        case 'a':
            strcat(word, "1010");
            break;
        case 'B':
        case 'b':
            strcat(word, "1011");
            break;
        case 'C':
        case 'c':
            strcat(word, "1100");
            break;
        case 'D':
        case 'd':
            strcat(word, "1101");
            break;
        case 'E':
        case 'e':
            strcat(word, "1110");
            break;
        case 'F':
        case 'f':
            strcat(word, "1111");
            break;
        default:
            break;
        }

        i++;
    }

    /*     if (num < 0)
        {
            i = 3;
            while (i < BINARY_WORD_SIZE)
            {
                i++;
            }
        } */
    /*
     */
    strcat(word, "\0");
    return word;
}

int binary2Decimal(char binaryStr[4])
{
    /*
        if (!strcmp(binaryStr, '0000'))
            return 0;
        if (!strcmp(binaryStr, '0001'))
            return 1;
        if (!strcmp(binaryStr, '0010'))
            return 2;
        if (!strcmp(binaryStr, '0011'))
            return 3;
        if (!strcmp(binaryStr, '0100'))
            return 4;
        if (!strcmp(binaryStr, '0101'))
            return 5;
        if (!strcmp(binaryStr, '0110'))
            return 6;
        if (!strcmp(binaryStr, '0111'))
            return 7;
        if (!strcmp(binaryStr, '1000'))
            return 8;
        if (!strcmp(binaryStr, '1001'))
            return 9;
        if (!strcmp(binaryStr, '1010'))
            return 10;
        if (!strcmp(binaryStr, '1011'))
            return 11;
        if (!strcmp(binaryStr, '1100'))
            return 12;
        if (!strcmp(binaryStr, '1101'))
            return 13;
        if (!strcmp(binaryStr, '1110'))
            return 14;
        if (!strcmp(binaryStr, '1111'))
            return 15; */

    return 0;
}
char *hexToBin(char *hex)
{
    int i = 0;
    char *binaryStr = (char *)calloc(BINARY_WORD_SIZE + 1, sizeof(char *));

    while (hex[i] != '\0')
    {
        switch (hex[i])
        {

        case '0':
            strcat(binaryStr, " 0000");
            break;
        case '1':
            strcat(binaryStr, " 0001");
            break;
        case '2':
            strcat(binaryStr, " 0010");
            break;
        case '3':
            strcat(binaryStr, " 0011");
            break;
        case '4':
            strcat(binaryStr, " 0100");
            break;
        case '5':
            strcat(binaryStr, " 0101");
            break;
        case '6':
            strcat(binaryStr, " 0110");
            break;
        case '7':
            strcat(binaryStr, " 0111");
            break;
        case '8':
            strcat(binaryStr, " 1000");
            break;
        case '9':
            strcat(binaryStr, " 1001");
            break;
        case 'A':
        case 'a':
            strcat(binaryStr, " 1010");
            break;
        case 'B':
        case 'b':
            strcat(binaryStr, " 1011");
            break;
        case 'C':
        case 'c':
            strcat(binaryStr, " 1100");
            break;
        case 'D':
        case 'd':
            strcat(binaryStr, " 1101");
            break;
        case 'E':
        case 'e':
            strcat(binaryStr, " 1110");
            break;
        case 'F':
        case 'f':
            strcat(binaryStr, " 1111");
            break;
        default:
            break;
        }

        i++;
    }

    strcat(binaryStr, "\0");
    return binaryStr;
}
int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

Bool isOperation(char *s)
{

    return (getOperationByName(s) != NULL) ? True : False;
}

Bool isLabel(char *s)
{
    int len = strlen(s);
    if (len <= 1)
        return yieldError(illegalLabelNameLength);

    return s[len - 1] == ':' ? True : False;
}

int getInstructionType(char *s)
{
    if (!strcmp(s, DATA))
        return _TYPE_DATA;
    if (!strcmp(s, STRING))
        return _TYPE_STRING;
    if (!strcmp(s, ENTRY))
        return _TYPE_ENTRY;
    if (!strcmp(s, EXTERNAL))
        return _TYPE_EXTERNAL;
    return False;
}

Bool isInstruction(char *s)
{

    if ((!strcmp(s, DATA) || !strcmp(s, STRING) || !strcmp(s, ENTRY) || !strcmp(s, EXTERNAL)))
        return True;

    else if (strstr(s, DATA) != NULL || strstr(s, STRING) != NULL || strstr(s, ENTRY) != NULL || strstr(s, EXTERNAL) != NULL)
    {
        yieldError(missinSpaceAfterInstruction);
        return True;
    }
    else
        return False;
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

        if (getRegisteryNumber(s) < 10)
            return False;
    }
    return True;
}

Bool isComment(char *s)
{
    return s[0] == ';' ? True : False;
}

int getRegisteryNumber(char *s)
{
    int len = strlen(s);
    int i = 0;
    if (s[0] == 'r' && len >= 2)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(s, regs[i]) == 0))
                return i;
            i++;
        }
    }
    return -1;
}

char *getInstructionNameByType(int type)
{
    switch (type)
    {
    case _TYPE_DATA:
        return "DATA INSTRUCTION";

    case _TYPE_STRING:
        return "STRING INSTRUCTION";

    case _TYPE_ENTRY:
        return "ENTRY INSTRUCTION";

    case _TYPE_EXTERNAL:
        return "EXTERNAL INSTRUCTION";

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
    /*
        if (!strcmp(s, DATA))
            return DATA;
        if (!strcmp(s, STRING))
            return STRING;
        if (!strcmp(s, ENTRY))
            return ENTRY;
        if (!strcmp(s, EXTERNAL))
            return EXTERNAL; */
    return 0;
}
