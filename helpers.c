#include "data.h"
char *trimFromLeft(char *s)
{
    while (isspace(*s))
        s++;
    return s;
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

    strcat(word, "\0");
    return word;
}

int binary2Decimal(char binaryStr[4])
{

    if (!strcmp(binaryStr, "0000"))
        return 0;
    if (!strcmp(binaryStr, "0001"))
        return 1;
    if (!strcmp(binaryStr, "0010"))
        return 2;
    if (!strcmp(binaryStr, "0011"))
        return 3;
    if (!strcmp(binaryStr, "0100"))
        return 4;
    if (!strcmp(binaryStr, "0101"))
        return 5;
    if (!strcmp(binaryStr, "0110"))
        return 6;
    if (!strcmp(binaryStr, "0111"))
        return 7;
    if (!strcmp(binaryStr, "1000"))
        return 8;
    if (!strcmp(binaryStr, "1001"))
        return 9;
    if (!strcmp(binaryStr, "1010"))
        return 10;
    if (!strcmp(binaryStr, "1011"))
        return 11;
    if (!strcmp(binaryStr, "1100"))
        return 12;
    if (!strcmp(binaryStr, "1101"))
        return 13;
    if (!strcmp(binaryStr, "1110"))
        return 14;
    if (!strcmp(binaryStr, "1111"))
        return 15;

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
