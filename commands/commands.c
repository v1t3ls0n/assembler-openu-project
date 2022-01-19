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

char *getFirstWordByCommand(Command *cmd)
{
    return strcat(hexToBin((char *)A), hexToBin((char *)cmd->opMachineCodeHex));
}

Bool isLabelNameLegal(char *s)
{
    int i = 0;
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

char *hexToBin(char *hex)
{
    /* Jumping over the first 0x characters */
    int i = 2, size = (strlen(hex) - 2) * 4;
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

int main()
{
    isLabelNameLegal("r0");
    printf("%s", getFirstWordByCommand(getCommandByName("add")));

    return 0;
}