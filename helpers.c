#include "data.h"
char *splitToken(char *s)
{
    char *start = 0, *end;
    char *nextToken;
    s = trimFromLeft(s);
    nextToken = (char *)calloc(strlen(s) + 2, sizeof(char *));
    strcpy(nextToken, s);
    start = nextToken;
    end = start;
    while (*end != '\0' && !isspace(*end))
        end++;

    if (*end)
        *end = '\0';

    return start;
}

char *cloneString(char *s)
{
    char *copy;
    if (!(*s))
        return "";
    copy = (char *)calloc(strlen(s), sizeof(char *));
    strcpy(copy, s);
    return copy;
}

char *trimFromLeft(char *s)
{
    while (isspace(*s) && *s != '\0')
        s++;
    return s;
}

char *trimFromRight(char *s)
{
    long offset = strlen(s) - 1;
    char *start = s;
    s += offset;
    while (isspace(*s))
        s--;
    return start;
}
int countSpaceCharacters(char *s)
{
    int i = 0;
    for (i = 0; isspace(*s); s++, i++)
        ;
    return i;
}

unsigned calcNumberCharactersLength(int num)
{
    unsigned count = 1;
    while (num / 10)
        count++;

    return count;
}

char *decToHex(int num)
{
    int i = num, size = 0;
    char *hex;
    for (size = 0; i > 0; i = i / 16)
        size++;
    hex = (char *)calloc(size, sizeof(char *));
    sprintf(hex, "%05x", num);
    return hex;
}

char *hexToBin(char *hex)
{
    int i = 0;
    char *binaryStr = (char *)calloc(BINARY_WORD_SIZE + 1, sizeof(char *));

    while (hex[i] != '\0' && i < 5)
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

int countConsecutiveCommas(char *s)
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