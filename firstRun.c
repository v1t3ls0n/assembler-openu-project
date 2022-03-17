#include "data.h"
/* Shared global State variables*/
extern State globalState;
extern unsigned currentLine;

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
extern Bool countAndVerifyStringArguments(char *token);
extern Bool parseFile(FILE *fp, char *filename);

extern Bool writeStringInstruction(char *s);
extern Bool writeDataInstruction(char *s);

Bool handleOperation(char *operationName, char *args)
{
    Operation *p = getOperationByName(operationName);
    char comma = 0;
    char *first = 0, *second = 0, *inBetweenCharacters = 0, *extra = 0;
    AddrMethodsOptions active[2] = {{0, 0, 0, 0}, {0, 0, 0, 0}};

    first = strtok(args, " \t \n");
    inBetweenCharacters = strtok(NULL, " \t \n");

    if (!first && !inBetweenCharacters)
    {
    }
    else if (first && inBetweenCharacters != NULL && strlen(inBetweenCharacters) == 1 && inBetweenCharacters[0] == ',')
    {
        comma = ',';
        second = strtok(NULL, " \t \n");
    }
    else if (inBetweenCharacters != NULL)
        second = inBetweenCharacters;

    else
    {
        if (strchr(first, ','))
        {
            char *p = strchr(first, ',');
            second = p;
            second++;
            comma = ',';
            *p = '\0';
            /*
                     printf("line 208, first:%s, second:%s\n", first, second); */
        }
        else
        {
            second = first;
            first = 0;
        }
    }
    extra = strtok(NULL, " \t \n");
    if (extra)
        return yieldError(illegalApearenceOfExtraCharactersOnLine);

    if (parseOperands(first, comma, second, p, active))
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
        return True;
    }

    return False;
}

Bool parseOperands(char *src, char comma, char *des, Operation *op, AddrMethodsOptions active[2])
{
    int commasCount = 0;
    int expectedCommasBasedOnNumberOfOperands = 0;
    printf("inside parse operands\nsrc:%s\ndes:%s\n", src, des);
    expectedCommasBasedOnNumberOfOperands = (src && des) ? 1 : 0;
    if (src && strchr(src, ','))
    {
        char *p = strchr(src, ',');
        *p = '\0';

        commasCount++;
    }
    if (des && strchr(des, ','))
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
        if (!op->src.direct && !op->src.immediate && !op->src.index && !op->src.reg && !op->des.direct && !op->des.immediate && !op->des.index && !op->des.reg && !src && !des)
            return True;
        else if ((op->src.direct || op->src.immediate || op->src.reg || op->src.index) && (op->des.direct || op->des.immediate || op->des.reg || op->des.index))
        {
            if (!strlen(src))
                return yieldError(requiredSourceOperandIsMissin);
            if (!strlen(des))
                return yieldError(requiredDestinationOperandIsMissin);
            return validateOperandMatch(op->src, active, src, 0) && validateOperandMatch(op->des, active, des, 1);
        }
        else if (op->src.direct || op->src.immediate || op->src.reg || op->src.index)
        {
            if (!strlen(src))
                return yieldError(requiredSourceOperandIsMissin);
            return validateOperandMatch(op->src, active, src, 0);
        }
        else if (op->des.direct || op->des.immediate || op->des.reg || op->des.index)
        {
            if (!strlen(des))
                return yieldError(requiredDestinationOperandIsMissin);
            return validateOperandMatch(op->des, active, des, 1);
        }
    }
    return True;
}
Bool validateOperandMatch(AddrMethodsOptions allowedAddrs, AddrMethodsOptions active[2], char *operand, int type)
{
    Bool isImmediate = isValidImmediateParamter(operand);
    Bool isDirectIndex = isValidIndexParameter(operand);
    Bool isReg = isRegistery(operand);
    Bool isDirect = verifyLabelNaming(operand);

    if (!isReg && !isImmediate && !isDirect && !isDirectIndex)
        return yieldError(notEnoughArgumentsPassed);
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
    /*  printf("line 169, type: %s\nfirst token: %s\nnexttoken: %s\nline: %s\n", getInstructionNameByType(type), firstToken, nextTokens, line); */
    if (isInstruction(firstToken))
    {
        if (type == _TYPE_DATA)
            return countAndVerifyDataArguments(line);
        else if (type == _TYPE_STRING)
            return countAndVerifyStringArguments(nextTokens);

        if (type == _TYPE_ENTRY || type == _TYPE_EXTERNAL)
        {
            char *labelName = calloc(strlen(nextTokens), sizeof(char *));
            strcpy(labelName, nextTokens);

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

        if (((type == _TYPE_DATA && countAndVerifyDataArguments(line)) || (type == _TYPE_STRING && countAndVerifyStringArguments(nextTokens))) && isLabelNameAvailable)
            return addSymbol(firstToken, dataCounter, 0, 1, 0, 0);

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
        /*         if (instruction)
                { */
        if (instruction == _TYPE_ENTRY || instruction == _TYPE_EXTERNAL)
            return handleInstruction(instruction, nextToken, strtok(NULL, " \t \n"), line);
        else
            return handleInstruction(instruction, labelName, nextToken, line);
        /*        } */
    }

    else if (isOperation(nextToken))
    {
        int icAddr = getIC();
        char args[MAX_LINE_LEN] = {0};
        int offset = (int)(strlen(labelName) + strlen(nextToken) + 1);
        strcpy(args, &line[offset]);

        /* printf("args:%s\noffset:%d\n", args, offset);
         */
        /* printf("labelName:%s nextToken:%s line:%s\n", labelName, nextToken, line);
         */
        if (handleOperation(nextToken, args))
            return addSymbol(labelName, icAddr, 1, 0, 0, 0);
        else
            return False;
    }

    else
        yieldError(illegalLabelUseExpectedOperationOrInstruction);
    return False;
}
