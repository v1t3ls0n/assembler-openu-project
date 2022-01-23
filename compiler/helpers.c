#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "flags.h"
#define BINARY_WIDTH 16
char *decToHex(int num);
char *hexToBin(char *hex);
int hex2int(char ch);
char *dec2Bin2sComplement(int n);

char *decToHex(int num)
{
    int i = num, size = 0;
    char *hex;
    for (size = 0; i > 0; i = i / 16)
        size++;
    hex = (char *)calloc(size, sizeof(char));
    sprintf(hex, "%x", num);
    return hex;
}

char *dec2Bin2sComplement(int n)
{

    int i = 0, j, num;
    char bin[BINARY_WIDTH] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, carry = '0';
    printf("n:%d\n", n);
    num = n < 0 ? -n : n;
    num = abs(n);
    printf("%d as binary:(before flip):\n", num);

    for (i = 0; num > 0; i++)
    {
        if (i == BINARY_WIDTH)
            break;
        bin[i] = num % 2;
        num = num / 2;

        printf("%d", bin[i]);
    }
    for (j = 0; j < BINARY_WIDTH; j++)
    {
        if (j > 0 && j % 4 == 0)
            printf(" ");
    }
    printf("%d", bin[i]);

    for (i = BINARY_WIDTH - 1; i >= 0; i--)
    {
        bin[i] = bin[i] == 0 ? 1 : 0;
        if (bin[i] == 1 && carry == 1)
            bin[i] = 0;

        else if (bin[i] == 0 && carry == 1)
        {
            bin[i] = 1;
            carry = 0;
        }
    }

    printf("\nafter flipping :\n");
    for (i = 0; i < BINARY_WIDTH; i++)
        printf("%d", bin[i]);
    printf("\n");
    return "";
}

char *hexToBin(char *hex)
{
    int i = 0, size = strlen(hex) * 4;
    char *binaryStr = (char *)calloc(size + 1, sizeof(char *));
    while (hex[i])
    {
        switch (hex[i])
        {
        case '0':
            strcat(binaryStr, "0000");
            break;
        case '1':
            strcat(binaryStr, "0001");
            break;
        case '2':
            strcat(binaryStr, "0010");
            break;
        case '3':
            strcat(binaryStr, "0011");
            break;
        case '4':
            strcat(binaryStr, "0100");
            break;
        case '5':
            strcat(binaryStr, "0101");
            break;
        case '6':
            strcat(binaryStr, "0110");
            break;
        case '7':
            strcat(binaryStr, "0111");
            break;
        case '8':
            strcat(binaryStr, "1000");
            break;
        case '9':
            strcat(binaryStr, "1001");
            break;
        case 'A':
        case 'a':
            strcat(binaryStr, "1010");
            break;
        case 'B':
        case 'b':
            strcat(binaryStr, "1011");
            break;
        case 'C':
        case 'c':
            strcat(binaryStr, "1100");
            break;
        case 'D':
        case 'd':
            strcat(binaryStr, "1101");
            break;
        case 'E':
        case 'e':
            strcat(binaryStr, "1110");
            break;
        case 'F':
        case 'f':
            strcat(binaryStr, "1111");
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
