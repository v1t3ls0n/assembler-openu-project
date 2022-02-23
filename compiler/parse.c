#include "data.h"
void main() { printf("\n\n\n\n\n\nTest somefile.c (in fake main function)\n\n\n\n\n\n"); }

/*
// errors:

Starting build...
C:\Users\guyvi\dev\msys64\mingw64\bin\gcc.exe -fdiagnostics-color=always -g C:\Users\guyvi\dev\msys64\home\guyvi\compiler-openu\compiler\parse.c -o C:\Users\guyvi\dev\msys64\home\guyvi\compiler-openu\compiler\parse.exe
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\guyvi\AppData\Local\Temp\cc60rEk1.o: in function `handleCommand':
C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/parse.c:85: undefined reference to `getCommandByName'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/parse.c:86: undefined reference to `generateFirstWordEncodedHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/parse.c:86: undefined reference to `writeToMemory'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\guyvi\AppData\Local\Temp\cc60rEk1.o: in function `handleLabel':
C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/parse.c:119: undefined reference to `writeToMemory'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/parse.c:120: undefined reference to `addSymbol'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/parse.c:126: undefined reference to `getCommandByName'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/parse.c:126: undefined reference to `generateFirstWordEncodedHex'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/parse.c:126: undefined reference to `writeToMemory'
C:/Users/guyvi/dev/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/11.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/Users/guyvi/dev/msys64/home/guyvi/compiler-openu/compiler/parse.c:127: undefined reference to `addSymbol'
collect2.exe: error: ld returned 1 exit status

Build finished with error(s).
*/

int parseSingleLine(char *line)
{
    int n;
    char token[MAX_LABEL_LEN + 1] = {0};
    char *p = line;
    ParseState state = newLine;

    while (*p)
    {
        sscanf(p, "%s %n", token, &n);

        state = evalToken(token, state);
        switch (state)
        {
        case parseLabel:
        {
            state = handleLabel(token, p + n);
            break;
        }

        case parseInstruction:
        {
            handleInstruction(token, p + n);
            break;
        }

        case parseCommand:
        {
            handleCommand(token, p + n);
            break;
        }
        case printError:
            return 0;
        case skipLine:
            return 1;

        default:
            break;
        }

        p += n;
    }

    return 1;
}

int evalToken(char *token, ParseState state)
{

    switch (state)
    {
    case skipLine:
        return state;
    case printError:
        return state;
    case newLine:
    {
        if (token[0] == ';')
            return skipLine;

        if (isLabel(token))
            return parseLabel;
        else if (isInstruction(token))
            return parseInstruction;

        else if (isCommand(token))
            return parseCommand;

        else
        {
            currentError = undefinedCommand;
            return printError;
        }
    }
    }

    return 1;
}

int handleCommand(char *cmdName, char *operands)
{
    Command *cmd = getCommandByName(cmdName);
    writeToMemory(*generateFirstWordEncodedHex(cmd), Code);
    return 1;
}

int handleInstruction(char *instruction, char *params)
{

    if (strcmp(instruction, DATA) == 0)
    {
        printf("Data instruction. params:%s\n", params);
    }
    if (strcmp(instruction, STRING) == 0)
    {
        printf("String instruction. params:%s\n", params);
    }
    if (strcmp(instruction, ENTRY) == 0)
    {
        printf("Entry instruction. params:%s\n", params);
    }
    if (strcmp(instruction, EXTERNAL) == 0)
    {
        printf("External instruction. params:%s\n", params);
    }

    return 1;
}

int handleLabel(char *labelName, char *nextToken)
{
    EncodedWord word = {0, 0, 0, 0, 0};
    int val;
    if (isInstruction(nextToken))
    {
        val = writeToMemory(word, Code);
        addSymbol(labelName, val, 0, 1, 0, 0);
        return parseInstruction;
    }

    else if (isCommand(nextToken))
    {
        val = writeToMemory(*generateFirstWordEncodedHex(getCommandByName(nextToken)), Code);
        addSymbol(labelName, val, 1, 0, 0, 0);

        handleCommand(labelName, nextToken);

        return parseCommand;
    }

    else
    {
        currentError = illegalLabelUseExpectedCommandOrInstruction;
        return printError;
    }

    return 1;
}

int isCommand(char *s)
{
    int i = 0;
    const char *cmds[] = {
        "mov",
        "cmp",
        "add",
        "sub",
        "lea",
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "jsr",
        "red",
        "prn",
        "rts",
        "stop",
        NULL};

    while (cmds[i] != NULL)
    {
        if (strcmp(s, *cmds) == 0)
            return 1;
        i++;
    }
    return 0;
}

int isLabel(char *s)
{
    int len = strlen(s);
    return s[len] == ':' ? 1 : 0;
}
int isInstruction(char *s)
{
    return !strcmp(s, DATA) || !strcmp(s, STRING) || !strcmp(s, ENTRY) || !strcmp(s, EXTERNAL);
}
