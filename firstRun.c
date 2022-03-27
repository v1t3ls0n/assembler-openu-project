#include "data.h"
extern Bool yieldError(Error err);
extern Bool yieldWarning(Warning err);

extern Operation *getOperationByName(char *s);
extern Bool addSymbol(char *name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal);
extern Bool isLabelNameAlreadyTaken(char *name, ItemType type);
extern Bool verifyLabelNaming(char *s);
extern void increaseDataCounter(int amount);
extern void increaseInstructionCounter(int amount);
extern unsigned getDC();
extern unsigned getIC();
extern Bool writeOperationBinary(char *operationName, char *args);

/* parse.c */
extern Bool countAndVerifyDataArguments(char *line);
extern Bool countAndVerifyStringArguments(char *line);

extern void parseAssemblyCode(FILE *src);

extern Bool writeStringInstruction(char *s);
extern Bool writeDataInstruction(char *s);
extern Bool verifyCommaSyntax(char *line);
char *getNextToken(char *s);
char *splitToken(char *s);
char *trimFromRight(char *s);
Bool isLabelDeclarationStrict(char *s);
Bool isOperationNotStrict(char *s);
char *getOperationName(char *s);
Bool handleOperation(char *operationName, char *args)
{
    Operation *p = getOperationByName(operationName);
    AddrMethodsOptions active[2] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
    char *first = 0;
    char *second = 0;
    char *extra = 0;
    Bool areOperandsLegal;

    if (*args)
        areOperandsLegal = verifyCommaSyntax(args);

    first = strtok(args, ", \t\n\f\r");
    if (first)
    {
        second = strtok(NULL, ", \t\n\f\r");
        if (second)
        {
            extra = strtok(NULL, ", \t\n\f\r");
            if (extra)
                yieldError(extraOperandsPassed);
        }
        else
        {
            second = 0;
            /*             second = first;
                        first = 0; */
        }
    }

    areOperandsLegal = parseOperands(first, second, p, active) && areOperandsLegal;

    if (areOperandsLegal)
    {
        int size = 2;
        if (active[0].immediate || active[1].immediate)
            size++;
        if ((active[0].direct || active[0].index) || (active[1].direct || active[1].index))
            size += 2;
        if (!p->funct && (!active[0].direct && !active[0].immediate && !active[0].index && !active[0].reg) && (!active[1].direct && !active[1].immediate && !active[1].index && !active[1].reg))
            size = 1;

        active[0].direct = active[0].immediate = active[0].index = active[0].reg = 0;
        active[1].direct = active[1].immediate = active[1].index = active[1].reg = 0;
        increaseInstructionCounter(size);
    }

    return areOperandsLegal ? True : False;
}
Bool parseOperands(char *src, char *des, Operation *op, AddrMethodsOptions active[2])
{
    int expectedOperandsCount = 0;
    int operandsPassedCount = 0;
    Bool isValid = True;
    if (src)
        operandsPassedCount++;
    if (des)
        operandsPassedCount++;
    if (op->src.direct || op->src.immediate || op->src.index || op->src.reg)
        expectedOperandsCount++;
    if (op->des.direct || op->des.immediate || op->des.index || op->des.reg)
        expectedOperandsCount++;

    if (expectedOperandsCount == 1 && operandsPassedCount == 1)
    {
        des = src;
        src = 0;
    }

    if ((expectedOperandsCount == operandsPassedCount) && expectedOperandsCount == 0)
        return True;

    if (operandsPassedCount > expectedOperandsCount)
        isValid = yieldError(extraOperandsPassed);

    if ((op->src.direct || op->src.immediate || op->src.reg || op->src.index) && (op->des.direct || op->des.immediate || op->des.reg || op->des.index))
    {

        if (!src)
            isValid = yieldError(requiredSourceOperandIsMissin);

        else
            isValid = validateOperandMatch(op->src, active, src, 0) && isValid;

        if (!des)
            isValid = yieldError(requiredDestinationOperandIsMissin);
        else
            isValid = validateOperandMatch(op->des, active, des, 1) && isValid;
    }
    else if (op->src.direct || op->src.immediate || op->src.reg || op->src.index)
    {
        if (!src)
            return yieldError(requiredSourceOperandIsMissin);
        else
            return validateOperandMatch(op->src, active, src, 0) && isValid;
    }
    else if (op->des.direct || op->des.immediate || op->des.reg || op->des.index)
    {
        if (!des)
            return yieldError(requiredDestinationOperandIsMissin);
        else
            return validateOperandMatch(op->des, active, des, 1) && isValid;
    }

    return isValid;
}
Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, AddrMethodsOptions active[2], char *operand, int type)
{
    Bool isImmediate = isValidImmediateParamter(operand);
    Bool isDirectIndex = !isImmediate && isValidIndexParameter(operand);
    Bool isReg = !isDirectIndex && !isImmediate && isRegistery(operand);
    Bool isDirect = !isReg && !isDirectIndex && !isImmediate && verifyLabelNaming(operand);

    if (!isReg && !isImmediate && !isDirect && !isDirectIndex)
        return type == 1 ? yieldError(desOperandTypeIsNotAllowed) : yieldError(srcOperandTypeIsNotAllowed);

    else if (!allowedAddrs.reg && isReg)
        return yieldError(operandTypeDoNotMatch);
    else if (!allowedAddrs.immediate && isImmediate)
        return yieldError(operandTypeDoNotMatch);
    else if (!allowedAddrs.direct && isDirect)
        return yieldError(illegalOperand);
    else if (!allowedAddrs.index && isDirectIndex)
        return yieldError(operandTypeDoNotMatch);

    active[type].direct = isDirect;
    active[type].reg = isReg;
    active[type].immediate = isImmediate;
    active[type].index = isDirectIndex;

    return True;
}

Bool handleInstruction(int type, char *firstToken, char *nextTokens, char *line)
{

    if (isInstruction(firstToken))
    {

        if (type == _TYPE_DATA)
        {
            return countAndVerifyDataArguments(line) ? True : False;
        }
        else if (type == _TYPE_STRING)
            return countAndVerifyStringArguments(line) ? True : False;

        if (type == _TYPE_ENTRY || type == _TYPE_EXTERNAL)
        {
            if (nextTokens)
            {
                char *labelName = (char *)calloc(strlen(nextTokens), sizeof(char *));
                strcpy(labelName, nextTokens);
                nextTokens = strtok(NULL, " \t\n\f\r");
                if (nextTokens)
                {
                    yieldError(illegalApearenceOfCharactersInTheEndOfTheLine);
                    return False;
                }
                else
                {
                    if (type == _TYPE_ENTRY)
                        return addSymbol(labelName, 0, 0, 0, 1, 0) ? True : False;
                    if (type == _TYPE_EXTERNAL)
                        return addSymbol(labelName, 0, 0, 0, 0, 1) ? True : False;
                }

                free(labelName);
            }
            else
            {
                yieldError(emptyDeclaretionOfEntryOrExternalVariables);
                return False;
            }
        }
    }
    else if (isLabelDeclaration(firstToken))
    {
        int dataCounter = getDC();
        Bool isLabelNameAvailable;
        isLabelNameAvailable = !isLabelNameAlreadyTaken(firstToken, Symbol);
        if (!isLabelNameAvailable)
            yieldError(illegalSymbolNameAlreadyInUse);

        if (((type == _TYPE_DATA && countAndVerifyDataArguments(line)) || (type == _TYPE_STRING && countAndVerifyStringArguments(line))) && isLabelNameAvailable)
        {
            return addSymbol(firstToken, dataCounter, 0, 1, 0, 0) ? True : False;
        }
    }

    return False;
}
Bool handleLabel(char *labelName, char *nextToken, char *line)
{
    Bool isValid = True;
    char *firstToken = 0;
    nextToken = trimFromLeft(nextToken);
    firstToken = nextToken;

    if (nextToken[0] == ':')
        nextToken++;
    nextToken = splitToken(nextToken);
    if (isInstruction(nextToken))
    {
        int instruction = getInstructionType(nextToken);
        int len = strlen(getInstructionNameByType(instruction)) + 1;
        char cleanInstruction[MAX_INSTRUCTION_NAME_LEN + 1] = {0}, *end = 0;
        strncpy(cleanInstruction, nextToken, len);
        end = cleanInstruction;
        while (!isspace(*end) && *end != '\0')
            end++;
        if (isspace(*end))
            *end = '\0';

        if (strlen(getInstructionNameByType(instruction)) != strlen(cleanInstruction))
        {
            isValid = yieldError(missinSpaceAfterInstruction);
            nextToken = nextToken + strlen(getInstructionNameByType(instruction));
        }

        if (instruction == _TYPE_ENTRY || instruction == _TYPE_EXTERNAL)
        {
            if (nextToken)
                return handleInstruction(instruction, firstToken, nextToken, line) && isValid;
            else
                return yieldWarning(emptyLabelDecleration);
        }
        else
            return handleInstruction(instruction, labelName, nextToken, line) && isValid;
    }

    else if (isOperation(nextToken))
    {
        int icAddr = getIC();
        char args[MAX_LINE_LEN] = {0};
        strcpy(args, (strstr(line, nextToken) + strlen(nextToken)));
        printf("args:%s\n", args);

        if (handleOperation(nextToken, args))
            return addSymbol(labelName, icAddr, 1, 0, 0, 0) ? True : False;
        else
            return False;
    }
    else
        return yieldError(undefinedLabelDeclaretion);

    return False;
}

char *getNextToken(char *s)
{
    char *start = s;
    while (*start != '\0' && !isspace(*start) && *start != ',')
        start++;
    return start;
}

char *splitToken(char *s)
{
    char *start = 0, *end;
    char *nextToken;
    s = trimFromLeft(s);
    nextToken = (char *)calloc(strlen(s) + 1, sizeof(char *));
    strcpy(nextToken, s);
    start = nextToken;
    end = start;
    while (*end != '\0' && !isspace(*end))
        end++;

    if (*end)
        *end = '\0';

    return start;
}