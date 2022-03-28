
#include "data.h"

extern void writeToCurrentExternalsFile(char *name, unsigned base, unsigned offset);

/* from firstRun.c */
extern Bool handleSingleLine(char *line);
extern Bool handleFirstToken(char *token, char *line, Bool state);
extern int getInstructionType(char *s);
extern Bool isOperation(char *s);
extern Bool isValidIndexParameter(char *s);
extern Bool isRegistery(char *s);
extern Bool isValidImmediateParamter(char *s);
extern Bool isComment(char *s);

/* from table.c: */
extern int getSymbolBaseAddress(char *name);
extern int getSymbolOffset(char *name);
extern Bool isExternal(char *name);

extern Bool isEntry(char *name);
extern Bool isNonEmptyEntry(char *name);
extern Bool areExternalsExist();
extern void updateExtPositionData(char *name, unsigned base, unsigned offset);
/* from operation.c */
extern Operation *getOperationByName(char *s);

/* from memory.c */
extern unsigned getDC();
extern unsigned getIC();
extern void addWord(int value, DataType type);
extern void parseAssemblyCode(FILE *src);

extern Bool parseLine(char *token, char *line);
extern Bool handleSingleLine(char *line);
void writeAdditionalOperandsWords(Operation *op, AddrMethodsOptions active, char *value);
/* @ Function: writeOperationBinary
   @ Arguments: The function gets char *operationName- the name of the operation and char *args - the arguments of the operation
   @ Description: The function splits char * args to tooken- each token is a different operand.
   Than the function checks what is the kind of the operands and writes them in words at the memory.
   If there is an operand which is an undifined labl at the table of symbols, it returns false. Else it returns true.
*/
Bool writeOperationBinary(char *operationName, char *args)
{
    Operation *op = getOperationByName(operationName);
    char *first, *second;
    AddrMethodsOptions active[2] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
    first = strtok(args, ", \t\n\f\r");
    second = strtok(NULL, ", \t\n\f\r");
    writeFirstWord(op);

    if (first && second && (detectOperandType(first, active, 0) && detectOperandType(second, active, 1)))
    {

        writeSecondWord(first, second, active, op);
        writeAdditionalOperandsWords(op, active[0], first);
        writeAdditionalOperandsWords(op, active[1], second);
    }
    else if (!second && first && detectOperandType(first, active, 1))
    {
        second = first;
        writeSecondWord(first, second, active, op);
        writeAdditionalOperandsWords(op, active[1], second);
    }
    else if (!first && !second && !op->funct)
        return True;

    else
        return False;

    return True;
}
/* @ Function: writeAdditionalOperandsWords
   @ Arguments: The function gets Operation *op- the operation, AddrMethodsOptions active- the allowed addresing methods for the operands,
   and char * value- the operand which is about to be written in the memory.
   @ Description: The functiom checks the addressing method of the operations, than acording to the addressing method it writes value (the operand) in the memory.
*/
void writeAdditionalOperandsWords(Operation *op, AddrMethodsOptions active, char *value)
{

    if (active.index)
    {
        parseLabelNameFromIndexAddrOperand(value);
        writeDirectOperandWord(value);
    }
    else if (active.direct)
        writeDirectOperandWord(value);
    else if (active.immediate)
        writeImmediateOperandWord(value);
}
/* @ Function: writeDataInstruction
   @ Arguments: The function gets char *token, which is the token of the .data instruction's arguments.
   @ Description: The function spilts char *token into tokens- separate each one of the .data arguments, then it adds and writes them in the memory.
   The function returns true.
*/
Bool writeDataInstruction(char *token)
{
    int num;
    while (token != NULL)
    {
        num = atoi(token);
        addWord((A << 16) | num, Data);
        token = strtok(NULL, ", \t\n\f\r");
    }
    return True;
}

/* @ Function: writeStringInstruction
   @ Arguments: The function gets char *s, which is the string of the .string instruction.
   @ Description: The function search the opening quotes in char *s, to find out where the string starts.
   Then, it it adds and writes the characters of the string in the memory.
   The function returns true.
*/
Bool writeStringInstruction(char *s)
{
    char *start = strchr(s, '\"');
    int i, len;
    start++;
    len = strlen(start);
    for (i = 0; i < len - 2; i++)
        addWord((A << 16) | start[i], Data);

    addWord((A << 16) | '\0', Data);
    return True;
}
/* @ Function: writeSecondWord
   @ Arguments: The function gets char *first and char *second - the first and second operands of the operation- Operation *op.
   It also gets AddrMethodsOptions active[2]- an array with the allowed addresing methods for the operands.
   @ Description: The function builds the second word of each operation and its parameters, it checks what is the
*/
void writeSecondWord(char *first, char *second, AddrMethodsOptions active[2], Operation *op)
{
    unsigned secondWord = (A << 16) | (op->funct << 12);
    if (first && (active[0].reg || active[0].index))
        secondWord = secondWord | (active[0].reg ? (getRegisteryNumber(first) << 8) : (parseRegNumberFromIndexAddrOperand(first) << 8)) | (active[0].reg ? (REGISTER_DIRECT_ADDR << 6) : (INDEX_ADDR << 6));
    else if (first && (active[0].direct || active[0].immediate))
        secondWord = secondWord | (0 << 8) | (active[0].direct ? (DIRECT_ADDR << 6) : (IMMEDIATE_ADDR << 6));
    if (second && (active[1].reg || active[1].index))
        secondWord = secondWord | (active[1].reg ? (getRegisteryNumber(second) << 2) : (parseRegNumberFromIndexAddrOperand(second) << 2)) | (active[1].reg ? (REGISTER_DIRECT_ADDR) : (INDEX_ADDR));
    else if (second && (active[1].direct || active[1].immediate))
        secondWord = secondWord | (0 << 2) | (active[1].direct ? (DIRECT_ADDR) : (IMMEDIATE_ADDR));

    addWord(secondWord, Code);
}

void writeFirstWord(Operation *op)
{
    unsigned firstWord = (A << 16) | op->op;
    addWord(firstWord, Code);
}

void writeDirectOperandWord(char *labelName)
{

    unsigned base = 0, offset = 0;
    if (isExternal(labelName))
    {
        base = getIC();
        addWord((E << 16) | 0, Code);
        offset = getIC();
        addWord((E << 16) | 0, Code);
        updateExtPositionData(labelName, base, offset);
    }

    else
    {
        base = getSymbolBaseAddress(labelName);
        offset = getSymbolOffset(labelName);
        addWord((R << 16) | base, Code);
        addWord((R << 16) | offset, Code);
    }
}

void writeImmediateOperandWord(char *n)
{
    n++;
    addWord((A << 16) | atoi(n), Code);
}

Bool detectOperandType(char *operand, AddrMethodsOptions active[2], int type)
{

    if (isRegistery(operand))
        active[type].reg = 1;
    else if (isValidImmediateParamter(operand))
        active[type].immediate = 1;
    else if (isValidIndexParameter(operand))
        active[type].index = 1;
    else
    {

        if (isSymbolExist(operand))
        {

            if (isEntry(operand) && !isNonEmptyEntry(operand))
                return yieldError(entryDeclaredButNotDefined);

            active[type].direct = 1;
        }
        else
            return yieldError(labelNotExist);
    }
    return True;
}

char *parseLabelNameFromIndexAddrOperand(char *s)
{
    char *p = strchr(s, '[');

    *p = 0;

    return s;
}

int parseRegNumberFromIndexAddrOperand(char *s)
{
    char *p = strchr(s, ']');

    s = strchr(s, '[');
    s++;
    if (p)
        *p = 0;

    return getRegisteryNumber(s);
}
