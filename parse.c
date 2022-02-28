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

extern void increaseDataCounter(int amount);
extern void inceaseInstructionCounter(int amount);
extern Item *updateSymbolAttribute(char *name, int type);
Bit binaryWord[BINARY_WORD_SIZE] = {0};

/*
                  IC -> 10
DC = Data Counter IC = Instruction counter
1 - comment -> skip
2 -> empty -> skip
3 -> error in line -> print specific error, change globalState to collect error, skip line
4 -> label or instruction -> check next token -> ? x: .data 3,3,5,67,7 DC
A) label is data/string/entry/external -> addSymbol(x, DC)
-> if(globalState == firstRun) upadteDataCounter(someWordAmount)
-> if(globalState == secondRun)writeToMemory(value=3,Data)
B) label is operation -> addSymbol(labelName, IC) ->
-> if(globalState == firstRun) upadteOperationCounter(someWordAmount)
-> if(globalState == secondRun) writeToMemory(word,Code) for each word of operation

*/

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
        if (state == newLine)
            state = handleState(token, p, state);

        switch (state)
        {
        case parseLabel:
        {
            state = handleLabel(token, strtok(NULL, " \t \n"), line);
            break;
        }

        case parseInstruction:
            state = handleInstruction(getInstructionType(token), token, line);
            break;

        case parseOperation:
        {
            state = handleOperation(getOperationByName(token), token + n, line);
            break;
        }
        case printError:
        {
            globalState = collectErrors;
            yieldError(currentError, lineNumber);
            return False;
        }
        case skipLine:
            return True;

        case True:
            return True;

        default:
            break;
        }

        token = strtok(NULL, " \t \n");
    }

    return True;
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
            return printError;

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

    Item *p = findOrAddSymbol(label, Symbol);
    printf("inside handle Instruction\ntype:%s\nlabelName:%s\nvalue:%s\n", getInstructionNameByType(type), label, nextTokens);

    if (p == NULL)
        return printError;

    if (type == _TYPE_ENTRY || type == _TYPE_EXTERNAL)
        return updateSymbolAttribute(label, type) != NULL ? True : printError;

    else
    {
        if (type == _TYPE_DATA)
        {
            int number;
            char *str = calloc(strlen(nextTokens), sizeof(char *));
            while ((sscanf(nextTokens, "%d%s", &number, str)) > 0)
            {

                printf("token:%s\ninteger number is:%d\n", str, number);
            }

            /*
            Word *newWord = NULL;
            memoryAddress = writeToMemory(newWord, Data);*/
        }
        else if (type == _TYPE_STRING)
        {
        }
    }
    /*
        if (type == _TYPE_DATA || type == _TYPE_STRING)
            {
                    int memoryAddress;
    Word *newWord = NULL;
                memoryAddress = writeToMemory(newWord, Data);
                }

    printf("inside handle Instruction\ntype:%s\nlabelName:%s\nvalue:%s\n", getInstructionNameByType(type), label, nextTokens);

    */
    return True;
}

int handleLabel(char *labelName, char *nextToken, char *line)
{
    int opIndex = -1;
    int memoryAddress = -1;
    /*cleaning the label token from the last : character as we should do before we try to add it to the symbol table.     */
    labelName[strlen(labelName) - 1] = '\0';
    if (addSymbol(labelName, 0, 0, 0, 0, 0) != NULL)
    {
        if (nextToken[0] == '.')
        {
            int instruction = getInstructionType(nextToken);
            if (instruction)
            {
                nextToken = strtok(NULL, " \t \n");
                return handleInstruction(instruction, labelName, nextToken);
            }
            else
            {
                currentError = undefinedInstruction;
                return printError;
            }
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
    }

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
    if (len <= 1)
    {
        currentError = illegalLabelNameLength;
        return False;
    }
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

char *getInstructionNameByType(int type)
{
    switch (type)
    {
    case _TYPE_DATA:
        return "DATA INSTRUCTION";

    case _TYPE_STRING:
        return "STRING INSTRUCTION";

    case _TYPE_ENTRY:
        return "ENTRY INSTRUCTION";

    case _TYPE_EXTERNAL:
        return "EXTERNAL INSTRUCTION";

    default:
        break;
    }

    return "NOT AN INSTRUCTION!";
}

/*
int calcLineMemoryUsage(Operation *op, char *srcOperand, char *desOperand)
{

    return 0;
}
void updateMemoryCounters() {}
*/
