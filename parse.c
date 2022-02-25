#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern Error currentError;
extern Item *symbols[HASHSIZE];
extern Item *macros[HASHSIZE];
/* Complex Struct Constant Variables: */
extern Operation operations[OP_SIZE];
extern Operation *getOperationByName(char *s);
extern Item *addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
extern Item *findOrAddSymbol(char *name, ItemType);
extern Item *findSymbol(char *name, ItemType type);

Bit binaryWord[BINARY_WORD_SIZE] = {0};

int parseSingleLine(char *line, int lineNumber)
{
    ParseState state = newLine;
    int n = 0;
    char *p = calloc(strlen(line + 1), sizeof(char *));
    char *token = calloc(MAX_LABEL_LEN, sizeof(char *));
    memcpy(p, line, strlen(line));
    token = strtok(p, " \t \n");

    printf("\t\t~ Currently parsing: ~\t\t\n\"%s\" (Line number 0%d)\n\n", line, lineNumber);

    while (token != NULL)
    {

        state = handleState(token, p, state);
        switch (state)
        {
        case parseLabel:
        {
            handleLabel(token, strtok(NULL, " \t \n"), line);
            break;
        }

        case parseInstruction:
            handleInstruction(getInstructionType(token), token, line);
            break;

        case parseOperation:
        {
            handleOperation(getOperationByName(token), token + n, line);
            break;
        }
        case printError:
        {
            globalState = collectErrors;
            yieldError(currentError, lineNumber);
            return False;
        }
        case skipLine:
            return 1;

        default:
            break;
        }

        token = strtok(NULL, " \t \n");
    }

    return 1;
}

int handleState(char *token, char *line, ParseState state)
{
    switch (state)
    {
    case skipLine:
        break;
    case newLine:
    {
        if (token[0] == ';')
            return skipLine;

        else if (isLabel(token))
        {
            return parseLabel;
        }

        else if (getInstructionType(token))
            return parseInstruction;

        else if (isOperation(token))
            return parseOperation;

        else
        {
            currentError = undefinedOperation;
            return printError;
        }

        break;
    }

    default:
        break;
    }

    return True;
}

int handleOperation(Operation *op, char *operands, char *line)
{

    printf("inside handleOperation\noperands:%s\n", operands);

    return 1;
}

int handleInstruction(int type, char *label, char *nextTokens)
{
    int memoryAddress;
    Word *newWord = NULL;
    Item *p = findSymbol(label, Symbol);

    if (type == _TYPE_DATA || type != _TYPE_STRING)
        memoryAddress = writeToMemory(newWord, Data);

    printf("inside handle Instruction, instruction type:%d labelName:%s nextTokens:%s\n", type, label, nextTokens);

    if (p == NULL)
    {
        p = addSymbol(label, memoryAddress, 0, 0, 0, 0);
        if (type == _TYPE_DATA || type == _TYPE_STRING)
            p->val.s.attrs.data = 1;
        else if (type == _TYPE_ENTRY)
            p->val.s.attrs.entry = 1;
        else if (type == _TYPE_EXTERNAL)
            p->val.s.attrs.external = 1;
    }

    else
    {

        if (((type == _TYPE_DATA || type == _TYPE_STRING) && p->val.s.attrs.code) || ((type == _TYPE_ENTRY) && p->val.s.attrs.external) || (type == _TYPE_EXTERNAL && p->val.s.attrs.entry))
        {
            currentError = symbolCannotBeBothCurrentTypeAndRequestedType;
            return printError;
        }
        if (type == _TYPE_DATA || type == _TYPE_STRING)
            p->val.s.attrs.data = 1;
        else if (type == _TYPE_ENTRY)
            p->val.s.attrs.entry = 1;
        else if (type == _TYPE_EXTERNAL)
            p->val.s.attrs.external = 1;
    }

    return True;
}

int handleLabel(char *labelName, char *nextToken, char *line)
{
    int opIndex = -1;
    int instruction = -1;
    int memoryAddress = -1;

    printf("inside handle Label, lable Name:%s nextToken:%s\n", labelName, nextToken);
    if ((instruction = getInstructionType(nextToken)))
    {
        printf("line 162, inside handleLabel\n");
        /*
                return handleInstruction(instruction, labelName, nextToken + strlen(nextToken)); */
    }

    else if (getOpIndex(nextToken) != -1)
    {
        Word *new = calloc(1, sizeof(Word *));
        new->value.hex = generateFirstWordEncodedHex(getOperationByIndex(opIndex));
        memoryAddress = writeToMemory(new, Code);
        if ((addSymbol(labelName, memoryAddress, 1, 0, 0, 0)) != NULL)
            return handleOperation(getOperationByIndex(opIndex), nextToken, line);
    }
    else
        currentError = illegalLabelUseExpectedOperationOrInstruction;

    return printError;
}

int isOperation(char *s)
{
    int opIndex = -1;
    opIndex = getOpIndex(s);
    printf("inside is operation op index:%d\n", opIndex);
    return opIndex != -1 ? opIndex : -1;
}

int isLabel(char *s)
{
    int len = strlen(s);
    if (len < 1)
        return False;
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

char *getInstructionName(char *s)
{
    if (!strcmp(s, DATA))
        return DATA;
    if (!strcmp(s, STRING))
        return STRING;
    if (!strcmp(s, ENTRY))
        return ENTRY;
    if (!strcmp(s, EXTERNAL))
        return EXTERNAL;
    return 0;
}

int calcLineMemoryUsage(Operation *op, char *srcOperand, char *desOperand)
{

    return 0;
}
void updateMemoryCounters() {}
