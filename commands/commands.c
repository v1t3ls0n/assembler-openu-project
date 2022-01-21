#include "table.h"

extern State globalState;
extern Error currentError;
Command commands[] = {
    {0x0001, 0, 0, "mov", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0002, 0, 1, "cmp", {1, 1, 1, 1}, {1, 1, 1, 1}},
    {0x0004, 10, 2, "add", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0004, 11, 2, "sub", {1, 1, 1, 1}, {0, 1, 1, 1}},
    {0x0010, 0, 4, "lea", {0, 1, 1, 0}, {0, 1, 1, 1}},
    {0x0020, 10, 5, "clr", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 11, 5, "not", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 12, 5, "inc", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0020, 13, 5, "dec", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x0200, 10, 9, "jmp", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 11, 9, "bne", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x0200, 12, 10, "jsr", {0, 0, 0, 0}, {0, 1, 1, 0}},
    {0x1000, 0, 12, "red", {0, 0, 0, 0}, {0, 1, 1, 1}},
    {0x2000, 0, 13, "prn", {0, 0, 0, 0}, {1, 1, 1, 1}},
    {0x4000, 0, 14, "rts", {0, 0, 0, 0}, {0, 0, 0, 0}},
    {0x8000, 0, 15, "stop", {0, 0, 0, 0}, {0, 0, 0, 0}},
};
void verifyLabelNaming(char *s);
Command *getCommandByName(char *s);

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
    return NULL; /*  */
}

void verifyLabelNaming(char *s)
{
    int i = 0;
    const char *regs[] = {R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15};
    int length = (int)(sizeof(regs) / sizeof(regs[0]));
    int labelLength = strlen(s);

    /* if label name does not start with a alphabet letter */
    if (isalpha(s[0]) == 0)
    {
        globalState = collectErrors;
        currentError = illegalLabelNameUseOfCharacters;
        return;
    }

    /* maximum label name length is 31 characters */
    if (strlen(s) > MAX_LABEL_LEN)
    {
        globalState = collectErrors;
        currentError = illegalLabelNameLength;
    }
    if (globalState != collectErrors && strchr(s, 'r') && labelLength >= 2 && labelLength <= 3)
    {
        while (i < length && globalState != collectErrors)
        {
            if ((strcmp(regs[i], s) == 0))
            {
                currentError = illegalLabelNameUseOfSavedKeywords;
                globalState = collectErrors;
            }
            i++;
        }
    }

    else if ((labelLength >= 3 && labelLength <= 4))
    {
        while (i < length)
        {
            if ((strcmp(commands[i].keyword, s) == 0))
            {
                currentError = illegalLabelNameUseOfSavedKeywords;
                globalState = collectErrors;
            }
            i++;
        }
    }
    else if (globalState != collectErrors)
    {

        while (i < labelLength && globalState != collectErrors)
        {
            if (!isdigit(s[i]))
            {
                currentError = illegalLabelNameUseOfCharacters;
                globalState = collectErrors;
            }
            i++;
        }
    }
}
