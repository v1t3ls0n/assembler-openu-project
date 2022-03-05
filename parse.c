#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern unsigned currentLine;
extern const char *regs[REGS_SIZE];
extern Operation operations[OP_SIZE];
extern Operation *getOperationByName(char *s);
extern Bool addSymbol(char *name, int value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
extern Bool isLabelNameAlreadyTaken(char *name, ItemType type);
extern Bool verifyLabelNaming(char *s);
extern void increaseDataCounter(int amount);
extern void increaseInstructionCounter(int amount);
extern unsigned getDC();
extern unsigned getIC();
extern void addNumberToMemory(int number);
static AddrMethodsOptions activeMethods[2];

int parseExpandedSourceFile(FILE *fp, char *filename)
{
    int c = 0;
    int i = 0;
    char line[MAX_LINE_LEN + 1] = {0};
    ParseState state = newLine;
    printf("inside parseExpandedSourceFile(FILE *fp, char *filename)\n");

    while (((c = fgetc(fp)) != EOF))
    {

        if (c == '\n')
        {
            parseSingleLine(line, state);
            memset(line, '\0', MAX_LINE_LEN);
            i = 0;
            state = newLine;
        }

        else if (i >= MAX_LINE_LEN - 1 && c != '\n')
        {
            globalState = collectErrors;
            return yieldError(maxLineLengthExceeded);
        }
        else if (isspace(c))
            line[i++] = ' ';

        else
        {
            if (isprint(c))
                line[i++] = c;
        }
    }

    return True;
}

int parseSingleLine(char *line, ParseState state)
{

    char *p = calloc(strlen(line + 1), sizeof(char *));
    char *token = calloc(MAX_LABEL_LEN, sizeof(char *));
    memcpy(p, line, strlen(line));
    token = strtok(p, " \t \n");
    printf("\ninside parseSingleLine, Line Number (%d):\n%s\n", currentLine, line);

    /*
     */
    if (state == newLine)
        state = handleState(token, p, state);

    while (token != NULL)
    {

        switch (state)
        {
        case lineParsedSuccessfully:
            state = True;
            break;
        case parseLabel:
        {

            state = handleLabel(token, strtok(NULL, " \t \n"), line);
            break;
        }

        case parseInstruction:
        {
            state = handleInstruction(getInstructionType(token), token, strtok(NULL, " \t \n"));
            break;
        }

        case parseOperation:
        {
            state = handleOperation(token, line);
            break;
        }

        case Err:
        {
            globalState = collectErrors;
            state = Err;
            break;
        }

        case skipLine:
            state = True;

        default:
            break;
        }
        token = strtok(NULL, " \t \n");
    }

    currentLine++;
    free(p);
    free(token);
    return state;
}

int handleState(char *token, char *line, ParseState state)
{
    /*   printf("inside handle State, token:%s\n", token); */
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

        else if (token[0] == '.')
            return parseInstruction;

        else if (isOperation(token))
            return parseOperation;
        else
            return yieldError(illegalApearenceOfCharactersOnLine);
        break;
    }

    default:
        break;
    }

    return True;
}

Bool handleOperation(char *operationName, char *line)
{
    Operation *p = getOperationByName(operationName);

    char firstOperand[MAX_LABEL_LEN] = {0}, secondOperand[MAX_LABEL_LEN] = {0};
    char comma = 0;
    int n = 0, nFirst = 0;
    line = operationName + strlen(operationName) + 1;
    sscanf(line, "%s%n%c%s%n", firstOperand, &nFirst, &comma, secondOperand, &n);
    if (secondOperand[0] == 0 && firstOperand[0] != '\0')
    {
        if (!strchr(firstOperand, ','))
        {
            memcpy(secondOperand, firstOperand, nFirst);
            firstOperand[0] = '\0';
        }
        else
        {
            memcpy(secondOperand, strrchr(firstOperand, ','), strlen(firstOperand));
            firstOperand[strlen(firstOperand) - strlen(secondOperand)] = 0;
        }
    }

    return parseOperands(firstOperand, comma, secondOperand, p);
}

Bool parseOperands(char *src, char comma, char *des, Operation *op)
{

    int commasCount = 0;
    int expectedCommasBasedOnNumberOfOperands = (strlen(src) > 0 && strlen(des) > 0) ? 1 : 0;

    printf("inside parse operands,expectedCommas:%d\nsrc:%s comma:%c des:%s\n", expectedCommasBasedOnNumberOfOperands, src, comma, des);

    if (src[strlen(src) - 1] == ',')
    {
        commasCount++;
        src--;
    }
    if (des[0] == ',')
    {
        commasCount++;
        des++;
    }
    if (comma == ',')
        commasCount++;

    if (commasCount > expectedCommasBasedOnNumberOfOperands)
        return yieldError(wrongInstructionSyntaxExtraCommas);

    else if (commasCount < expectedCommasBasedOnNumberOfOperands)
        return yieldError(wrongInstructionSyntaxMissinCommas);

    else if (commasCount == expectedCommasBasedOnNumberOfOperands)
    {
        if (!op->src.direct && !op->src.immediate && !op->src.index && !op->src.reg && !op->des.direct && !op->des.immediate && !op->des.index && !op->des.reg && !strlen(src) && !strlen(des))
            return True;
        else if ((op->src.direct || op->src.immediate || op->src.reg || op->src.index) && (op->des.direct || op->des.immediate || op->des.reg || op->des.index))
        {
            if (!strlen(src))
                return yieldError(requiredSourceOperandIsMissin);
            if (!strlen(des))
                return yieldError(requiredDestinationOperandIsMissin);
            return validateOperandMatch(op->src, src, 0) && validateOperandMatch(op->des, des, 1);
        }
        else if (op->src.direct || op->src.immediate || op->src.reg || op->src.index)
        {
            if (!strlen(src))
                return yieldError(requiredSourceOperandIsMissin);
            return validateOperandMatch(op->src, src, 0);
        }
        else if (op->des.direct || op->des.immediate || op->des.reg || op->des.index)
        {
            if (!strlen(des))
                return yieldError(requiredDestinationOperandIsMissin);
            return validateOperandMatch(op->des, des, 1);
        }
    }
    return True;
}
Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, char *operand, int type)
{
    Bool isImmediate = isValidImmediateParamter(operand);
    Bool isDirectIndex = isValidIndexParameter(operand);
    Bool isReg = isRegistery(operand);
    Bool isLabel = verifyLabelNaming(operand);

    if (!isReg && !isImmediate && !isLabel && !isDirectIndex)
        return yieldError(notEnoughArgumentsPassed);
    else if (!allowedAddrs.reg && isReg)
        return yieldError(operandTypeDoNotMatch);
    else if (!allowedAddrs.immediate && isImmediate)
        return yieldError(operandTypeDoNotMatch);
    else if (!allowedAddrs.direct && isLabel)
        return yieldError(illegalOperand);
    else if (!allowedAddrs.index && isDirectIndex)
        return yieldError(operandTypeDoNotMatch);

    activeMethods[type].direct = isDirectIndex;
    activeMethods[type].reg = isReg;
    activeMethods[type].immediate = isImmediate;
    activeMethods[type].index = isDirectIndex;

    return True;
}

int handleInstruction(int type, char *firstToken, char *nextTokens)
{
    /*
        printf("instructionType:%s firstToken:%s nextToken:%s\n", getInstructionNameByType(type), firstToken, nextTokens);
     */
    if (isInstruction(firstToken))
    {
        if (type == _TYPE_DATA)
            return handleInstructionDataArgs(nextTokens);
        else if (type == _TYPE_STRING)
            return handleInstructionStringArgs(nextTokens);

        if (type == _TYPE_ENTRY || type == _TYPE_EXTERNAL)
        {
            char *labelName = calloc(strlen(nextTokens), sizeof(char *));
            labelName = strdup(nextTokens);
            nextTokens = strtok(NULL, " \t \n");
            if (nextTokens)
                return yieldError(illegalApearenceOfCharactersOnLine);
            else
            {
                if (type == _TYPE_ENTRY)
                    return addSymbol(labelName, 0, 0, 0, 1, 0);
                if (type == _TYPE_EXTERNAL)
                    return addSymbol(labelName, 0, 0, 0, 0, 1);
            }
        }
    }
    else if (isLabel(firstToken))
    {
        int dataCounter = getDC();
        Bool isLabelNameAvailable;
        firstToken[strlen(firstToken) - 1] = '\0';
        isLabelNameAvailable = !isLabelNameAlreadyTaken(firstToken, Symbol);
        if (!isLabelNameAvailable)
            yieldError(illegalSymbolNameAlreadyInUse);

        if ((type == _TYPE_DATA && handleInstructionDataArgs(nextTokens)) || (type == _TYPE_STRING && handleInstructionStringArgs(nextTokens)))
            return isLabelNameAvailable ? addSymbol(firstToken, dataCounter, 0, 1, 0, 0) : False;
        else
            return Err;
    }

    return yieldError(undefinedOperation);
}

int handleLabel(char *labelName, char *nextToken, char *line)
{

    if (isInstruction(nextToken))
    {
        int instruction = getInstructionType(nextToken);
        if (instruction)
        {
            if (instruction == _TYPE_ENTRY || instruction == _TYPE_EXTERNAL)
                return handleInstruction(instruction, nextToken, strtok(NULL, " \t \n"));
            else
                return handleInstruction(instruction, labelName, nextToken);
        }
        else
            return yieldError(undefinedInstruction);
    }

    else if (isOperation(nextToken))
    {
        int icAddr = getIC();

        if (handleOperation(nextToken, line))
            return addSymbol(labelName, icAddr, 1, 0, 0, 0);
    }

    else
        yieldError(illegalLabelUseExpectedOperationOrInstruction);
    return False;
}

int isOperation(char *s)
{
    return (getOperationByName(s) != NULL) ? True : False;
}

int isLabel(char *s)
{
    int len = strlen(s);
    if (len <= 1)
        return yieldError(illegalLabelNameLength);

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

Bool isInstruction(char *s)
{
    return (!strcmp(s, DATA) || !strcmp(s, STRING) || !strcmp(s, ENTRY) || !strcmp(s, EXTERNAL)) ? True : False;
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

int handleInstructionDataArgs(char *token)
{
    int number = 0;
    int counter = 0;
    char illegalCharacter = 0;
    Bool minusSignOn = False;
    Bool commaState = True;
    if (isInstruction(token))
        token = strtok(NULL, " \t \n");

    while (token)
    {
        if (token[0] != '-')
        {
            minusSignOn = True;
            token = strtok(NULL, " \t \n");
        }

        else if (!isdigit(token[0]) && (token[0] != ',' || (token[0] != '+' && minusSignOn)))
        {

            if (isalpha(token[0]))
                return yieldError(expectedNumber);
            else
                return yieldError(illegalApearenceOfCharactersOnLine);
        }

        else if (token[0] == ',' && commaState)
            return yieldError(wrongInstructionSyntaxExtraCommas);

        else
        {
            if (commaState)
            {
                sscanf(token, "%d%c", &number, &illegalCharacter);

                if (illegalCharacter == '.')
                    return yieldError(wrongArgumentTypeNotAnInteger);
                else if (illegalCharacter == ',')
                    commaState = True;
                else
                    commaState = False;

                if (globalState == secondRun)
                {
                    if (minusSignOn)
                        number = -1 * number;
                    minusSignOn = False;
                    addNumberToMemory(number);
                }
                else
                    counter++;
            }
            else if (!commaState && strtok(NULL, " \t \n"))
            {
                return yieldError(expectedSingleCommaCharacter);
            }
            else
                return yieldError(expectedSingleCommaCharacter);
        }
        token = strtok(NULL, " \t \n");
    }

    if (globalState != secondRun)
        increaseDataCounter(counter);

    return lineParsedSuccessfully;
}

int handleInstructionStringArgs(char *token)
{
    if (isInstruction(token))
        token = strtok(NULL, " \t \n");

    /*

    printf("inside handle instruction string args, token:%s\n", token);

     */

    if ((token[0] == '\"') && !(token[strlen(token) - 1] == '\"'))
        return yieldError(closingQuotesForStringIsMissing);
    else if ((token[0] != '\"'))
        return yieldError(expectedQuotes);

    if (globalState == secondRun)
    {
        int i = 1;
        while (i < strlen(token) - 2)
        {
            addNumberToMemory((int)token[i]);
            i++;
        }
        addNumberToMemory((int)'\0');
    }
    else
        increaseDataCounter((int)(strlen(token) - 2));

    return lineParsedSuccessfully;
}

const char *getRegisteryOperand(char *s)
/*returns a constant- the name of the register, so it can b*/
{

    int len = strlen(s);
    int i = 0;

    if (s[0] == 'r' && len >= 2)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(regs[i], s) == 0))
                return regs[i];
            i++;
        }
        yieldError(wrongRegisteryReferenceUndefinedReg);
    }

    return 0;
}

Bool isRegistery(char *s)
{
    int len = strlen(s);
    int i = 0;
    if (s[0] == 'r' && len >= 2)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(regs[i], s) == 0))
                return True;
            i++;
        }
    }
    return False;
}

Bool isValidImmediateParamter(char *s)
{
    int i, len = strlen(s);
    if (len < 2 || s[0] != '#' || (!(s[1] == '-' || s[1] == '+' || isdigit(s[1]))))
        return False;
    for (i = 2; i < len; i++)
        if (!isdigit(s[i]))
            return False;
    return True;
}

Bool isValidIndexParameter(char *s)
{
    int len = strlen(s);
    if (len < 6)
        return False;
    else if (!(s[len - 1] == ']' && s[len - 4] == 'r' && s[len - 5] == '['))
        return False;
    else
    {
        s = strchr(s, '[');
        s++;
        s[strlen(s) - 1] = 0;
        if (getRegisteryNumber(s) < 10)
            return False;
    }
    return True;
}
int getRegisteryNumber(char *s)
{
    int len = strlen(s);
    int i = 0;
    if (s[0] == 'r' && len >= 2)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(s, regs[i]) == 0))
                return i;
            i++;
        }
    }
    return -1;
}
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
