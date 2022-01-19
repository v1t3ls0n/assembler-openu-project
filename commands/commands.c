#include "commands.h"

Command *getCommandByName(char *s)
{
    int i = 0;
    int length = (int)(sizeof(commands) / sizeof(commands[0]));
    while (i < length)
    {
        if (strcmp(commands[i].keyword, s) == 0)
            return &commands[i];
        i++;
    }
    return NULL;
}

char *getFirstWord(Command *cmd)
{
    return strcat(hexToBin(decToHex(A)), hexToBin(decToHex(cmd->opMachineCodeHex)));
}

Bool isLabelNameLegal(char *s)
{
    int i = 0;
    const char *regs[] = {R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15};
    int length = (int)(sizeof(regs) / sizeof(regs[0]));

    /* if label name does not start with a alphabet letter */
    if (isalpha(s[0]) == 0)
        return False;

    /* maximum label name length is 31 characters */
    if (strlen(s) > 31)
        return False;

    while (i < length)
    {
        if ((strcmp(regs[i], s) == 0) || (strcmp(commands[i].keyword, s) == 0))
            return False;
        i++;
    }
    return True;
}
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

void printObjectFile(Word obj[], unsigned int ICF, unsigned int DCF)
{
    int i;
    printf("              %d   %d\n", ICF, DCF);
    for (i = 0; i < (DCF + ICF); i++) /*  */
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", 100 + i, obj[i].POS_A, obj[i].POS_B, obj[i].POS_C, obj[i].POS_D, obj[i].POS_E);
}

void printBinaryFile(Word obj[], unsigned int ICF, unsigned int DCF)
{
    int i;
    for (i = 0; i < (DCF + ICF); i++)
    {
        printf("%04d ", 100 + i);
        printf("%s", hexToBin(decToHex(obj[i].POS_A)));
        printf("%s", hexToBin(decToHex(obj[i].POS_B)));
        printf("%s", hexToBin(decToHex(obj[i].POS_C)));
        printf("%s", hexToBin(decToHex(obj[i].POS_D)));
        printf("%s\n", hexToBin(decToHex(obj[i].POS_E)));
    }
}

int main()
{
    long size = sizeof(commands) / 16;
    Word machineCodeObj[] = {
        {0x2, 0x1, 0x3, 0xF, 0x1},
        {0x4, 0x2, 0x3, 0x9, 0x1},
        {0x7, 0x8, 0x6, 0xD, 0x1},
        {0x3, 0x1, 0x5, 0xE, 0x1},
        {0x2, 0x4, 0x3, 0xC, 0x1},
        {0x4, 0x0, 0x3, 0xC, 0x3},
        {0x4, 0x0, 0x3, 0xB, 0x7},
        {0x4, 0x0, 0x3, 0xE, 0x2},
        {0x4, 0x0, 0x3, 0xA, 0x6},
    };

    printf("commands size v1:%ld\n", size);
    size = sizeof(Word);
    printObjectFile(machineCodeObj, 6, 3);
    printBinaryFile(machineCodeObj, 6, 3);

    return 0;
}
