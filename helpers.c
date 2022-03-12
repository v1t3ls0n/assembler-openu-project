#include "data.h"
extern HexWord *convertNumToHexWord(int num);
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

char *dec2Bin2sComplement(int n)
{
    unsigned char result;

    /*
    Function converts decimal integer to binary/hex representation in a 2'Complement
    format, we use the first technic type of converting integers to 2'Complement numbers.
    Algorthim:
    1 - taking the absolute value of the number argument (n)
    2 - doing a NOT bitwise operation on all bits
    3 - adding 1 to the result
    4 - saving it as a string of chracers and as HexWord Typedef struct variable
    */
    if (n < 0)
    {
        result = abs(n);
        result = ~result;
        result++;

        printf("original number:%d\nresult in 2s complement:%s\n", n, numToBin(result));
    }

    return n < 0 ? numToBin(result) : numToBin(n);
}

char *numToBin(int num)
{
    int i = 0;
    char *word = (char *)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
    char *hex = decToHex(num);
    printf("hex:%s\n", hex);

    while (hex[i] != 0)
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
