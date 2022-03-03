#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern unsigned currentLine;
extern const char *regs[REGS_SIZE];

/* Complex struct *Constant Variables: */
/* Complex Struct Constant Variables: */
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
    int n = 0;
    line = operationName + strlen(operationName) + 1;
    sscanf(line, "%s%c%s%n", firstOperand, &comma, secondOperand, &n);
    printf("operationName:%s\nFirst Operand:%s\nSecond Operand:%s\ncomma:%c\nn:%d\n", operationName, firstOperand, secondOperand, comma, n);
    return parseOperands(firstOperand, comma, secondOperand, p);
}

Bool parseOperands(char *src, char comma, char *des, Operation *op)
{
    Bool isValid = True;
    /*     AddrMethodsOptions sourceAddr = {0, 0, 0, 0}, des = {0, 0, 0, 0}; */
    /*    printf("Operation allowed operands types:\n");
    printf("Source: immediate:%d direct:%d index:%d regDirect:%d\n", op->src.immediate, op->src.direct, op->src.index, op->src.reg);
       printf("Destination: immediate:%d direct:%d index:%d regDirect:%d\n\n\n", op->des.immediate, op->des.direct, op->des.index, op->des.reg); */
    if (!op->src.direct && !op->src.immediate && !op->src.index && !op->src.reg && !op->des.direct && !op->des.immediate && !op->des.index && !op->des.reg && !strlen(src) && !strlen(des))
        return True;

    else if ((op->src.direct || op->src.immediate || op->src.reg || op->src.index) && (op->des.direct || op->des.immediate || op->des.reg || op->des.index))
    {
        if (!strlen(src))
            isValid = yieldError(requiredSourceOperandIsMissin);
        if (!strlen(des))
            isValid = isValid && yieldError(requiredDestinationOperandIsMissin);

        isValid = isValid && validateOperandMatch(op->src, src) && validateOperandMatch(op->des, des);
        isValid = isValid && checkLegalUseOfCommas(src, comma, des);
    }

    else if (op->src.direct || op->src.immediate || op->src.reg || op->src.index)
    {
        if (!strlen(src))
            isValid = yieldError(requiredSourceOperandIsMissin);
        else
            isValid = validateOperandMatch(op->src, src);
    }
    else if (op->des.direct || op->des.immediate || op->des.reg || op->des.index)
    {
        if (!strlen(des))
            isValid = yieldError(requiredDestinationOperandIsMissin);
        else
            isValid = validateOperandMatch(op->des, des);
    }

    return True;
    /*     return isValid;
     */
}

Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, char *operand)
{
    if (!allowedAddrs.reg && !allowedAddrs.direct && !allowedAddrs.immediate && !allowedAddrs.index && strlen(operand) > 0)
        return yieldError(operandTypeDoNotMatch);
    else if (!allowedAddrs.reg && isRegistery(operand))
        return yieldError(operandTypeDoNotMatch);
    else if (!allowedAddrs.immediate && operand[0] == '#')
        return yieldError(operandTypeDoNotMatch);
    if (allowedAddrs.direct && !verifyLabelNaming(operand)) /*checks if the label's name is legal*/
        return yieldError(illegalOperand);

    /*     typedef struct
        {
            unsigned int immediate : 1;
    # 4 or #- 3 integers unsigned int direct : 1;
            label / entry / external unsigned int index : 1;
            someVar[r2] unsigned int reg : 1;
            r01 - r15
        } AddrMethodsOptions;

        if (allowedAddrs)

             */
    return True;
}

Bool checkLegalUseOfCommas(char *s1, char comma, char *s2)
{
    if (s1 && s2)
    {
        if ((s1[strlen(s1) - 1] != ',' && s2[0] == ',') || (s1[strlen(s1) - 1] == ',' && s2[0] != ','))
            return True;

        else if (s1[strlen(s1) - 1] == ',' && s2[0] == ',')
            return yieldError(wrongInstructionSyntaxExtraCommas);
        else if (s1[strlen(s1) - 1] != ',' && s2[0] != ',')
            return yieldError(expectedSingleCommaCharacter);
    }
    else if (strchr(s1, ',') != NULL)
        return yieldError(wrongInstructionSyntaxExtraCommas);

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
    /*     printf("in handle Label labelName:%s nextToken:%s\n", labelName, nextToken);
     */
    if (nextToken[0] == '.')
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

    return yieldError(illegalLabelUseExpectedOperationOrInstruction);
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
{

    int len = strlen(s);
    int i = 0;

    if (strchr(s, 'r') && len >= 2 && len <= 3)
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

    if (strchr(s, 'r') && len >= 2 && len <= 3)
    {
        while (i < REGS_SIZE)
        {
            if ((strcmp(regs[i], s) == 0))
                return True;
            i++;
        }
        return yieldError(wrongRegisteryReferenceUndefinedReg);
    }
    return False;
}

Bool isValidImmediateParamter(char *s)
{
    /*
    check #

    */

    return True;
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
