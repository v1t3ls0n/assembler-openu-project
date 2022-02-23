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

int parseSingleLine(char *line, int lineNumber)
{
    ParseState state = newLine;
    int n = 0;

    /*char token[MAX_LABEL_LEN + 1] = {0};*/
    char *token = (char *)calloc(MAX_LABEL_LEN + 1, sizeof(char));

    char *p = line;

    printf("\t\t~ Currently parsing: ~\t\t\n\"%s\" (Line number 0%d)\n\n", line, lineNumber);
    while (*p)
    {
        sscanf(p, "%s %n", token, &n);
        state = handleState(token, state);

        switch (state)
        {
        case parseLabel:
        {
            /* cleaning the label token from the last : character as we should do before we try to add it to the symbol table*/
            token[strlen(token) - 1] = '\0';
            handleLabel(token, p + n, line);

            break;
        }

        case parseInstruction:
            handleInstruction(isInstruction(token), token + n, line);
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

        p += n;
    }

    return 1;
}

int handleState(char *token, ParseState state)
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
            return parseLabel;

        else if (isInstruction(token))
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
        /*

    case parseInstruction:
        handleInstruction() break;
    case parseOperation:
        break;
*/
    default:
        break;
    }

    return True;
}

int handleOperation(Operation *op, char *operands, char *line)
{

    printf("inside handleOperation:\nOperation Name:%s Operands:%s line:%s\n", op->keyword, operands, line);

    return 1;
}

int handleInstruction(int type, char *label, char *nextTokens)
{
    EncodedWord newWord = {0, 0, 0, 0, 0};
    Item *p = findSymbol(label, Symbol);
    int memoryAddress = writeToMemory(newWord, Data);

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
    printf("inside handleLabel:\nLabel Name:%s nextToken:%s line:%s\n", labelName, nextToken, line);

    if ((instruction = isInstruction(nextToken)))
        return handleInstruction(getOpIndex(nextToken), labelName, nextToken + strlen(nextToken));

    else if ((opIndex = isOperation(nextToken)) != -1)
    {
        memoryAddress = writeToMemory(*generateFirstWordEncodedHex(getOperationByIndex(opIndex)), Code);
        if ((addSymbol(labelName, memoryAddress, 1, 0, 0, 0)) != NULL)
            return handleOperation(getOperationByIndex(opIndex), nextToken, line);
        else
            return printError;
    }
    else
    {
        currentError = illegalLabelUseExpectedOperationOrInstruction;
        return printError;
    }
}

int isOperation(char *s)
{
    int opIndex = getOpIndex(s);
    printf("inside is operation, token:%s op index:%d\n", s, opIndex);
    return opIndex != -1 ? opIndex : -1;
}

int isLabel(char *s)
{
    int len = strlen(s);
    return s[len - 1] == ':' ? True : False;
}

int isInstruction(char *s)
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

/*
    case parseLabel:
        break;
    case parseInstruction:
        break;
    case parseOperation:
        break;

    case parseDataVariable:
        break;
    case parseStringVariable:
        break;
    case parseEntryVariable:
        break;
    case parseExternalVariable:
        break;

    case parseSourceOperand:
        break;
    case parseDestinationOperand:
        break;
    case expectNewline:
        break;
    case expectNumber:
        break;
    case expectComma:
        break;
    case expectBlank:
        break;
    case expectQuotes:
        break;
        */