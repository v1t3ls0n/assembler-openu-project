#include "data.h"

Bool handleOperation(char *operationName, char *args)
{
    const Operation *p = getOperationByName(operationName);
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
                areOperandsLegal = yieldError(extraOperandsPassed);
        }
        else
            second = 0;
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

    return areOperandsLegal;
}
Bool parseOperands(char *src, char *des, const Operation *op, AddrMethodsOptions active[2])
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
    Bool isAny = isValidImmediateParamter(operand) || isValidIndexParameter(operand) || isRegistery(operand) || verifyLabelNaming(operand) || isIndexParameter(operand);
    Bool isImmediate = isValidImmediateParamter(operand);
    Bool isDirectIndex = !isImmediate && isValidIndexParameter(operand);
    Bool isReg = !isDirectIndex && !isImmediate && isRegistery(operand);
    Bool isDirect = !isReg && !isDirectIndex && !isImmediate && verifyLabelNaming(operand);

    if (isIndexParameter(operand) && !isDirectIndex)
        return yieldError(registeryIndexOperandTypeIfOutOfAllowedRegisteriesRange);

    if (!isAny)
        return type == 1 ? yieldError(illegalInputPassedAsOperandDesOperand) : yieldError(illegalInputPassedAsOperandSrcOperand);

    else if (!allowedAddrs.reg && isReg)
        return type == 1 ? yieldError(desOperandTypeIsNotAllowed) : yieldError(srcOperandTypeIsNotAllowed);
    else if (!allowedAddrs.immediate && isImmediate)
        return type == 1 ? yieldError(desOperandTypeIsNotAllowed) : yieldError(srcOperandTypeIsNotAllowed);
    else if (!allowedAddrs.direct && isDirect)
        return type == 1 ? yieldError(desOperandTypeIsNotAllowed) : yieldError(srcOperandTypeIsNotAllowed);
    else if (!allowedAddrs.index && isDirectIndex)
        return type == 1 ? yieldError(desOperandTypeIsNotAllowed) : yieldError(srcOperandTypeIsNotAllowed);

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
                char *labelName = cloneString(nextTokens);
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
        firstToken[strlen(firstToken) - 1] = '\0';
        isLabelNameAvailable = !isLabelNameAlreadyTaken(firstToken, Symbol);
        if (!isLabelNameAvailable)
            yieldError(illegalSymbolNameAlreadyInUse);

        if (((type == _TYPE_DATA && countAndVerifyDataArguments(line)) || (type == _TYPE_STRING && countAndVerifyStringArguments(line))) && isLabelNameAvailable)
        {

            return addSymbol(firstToken, dataCounter, 0, 1, 0, 0) ? True : False;
        }
        else
            return False;
    }
    else
        yieldError(undefinedOperation);

    return False;
}
Bool handleLabel(char *labelName, char *nextToken, char *line)
{
    Bool isValid = True;
    if (!labelName || !nextToken || !line)
        return False;
    if (isInstruction(nextToken))
    {
        int instruction = getInstructionType(nextToken);
        if (!isInstructionStrict(nextToken))
        {
            isValid = yieldError(missinSpaceAfterInstruction);
            nextToken = getInstructionNameByType(instruction);
        }

        if (instruction == _TYPE_ENTRY || instruction == _TYPE_EXTERNAL)
        {
            char *next = strtok(NULL, " \t\n\f\r");
            if (next)
                return handleInstruction(instruction, nextToken, next, line) && isValid;
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
        int offset = (int)(strlen(labelName) + strlen(nextToken) + 1);
        strcpy(args, &line[offset]);
        if (handleOperation(nextToken, args))
            return addSymbol(labelName, icAddr, 1, 0, 0, 0) ? True : False;
        else
            return False;
    }

    else
        yieldError(illegalLabelUseExpectedOperationOrInstruction);

    return False;
}