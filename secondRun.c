

/*
ignore for now:
if first token is label -> check if external if so -> update symbol table and add IC/DC to field of places of use.
 */

 /*

 *binaryImg is an Array of BinaryWords in the size of instruction and data words we counted

 Pseudo-Code:
 1. read line:

 2.if first token is ; comment or empty space, skip next.

 3.if label -> check if data/string instruction or operation

 4. if data/string instruction? go to step 5, if entry/external ignore and and skip line

 5.if data/string instruction, with iterate through arguments,
 every letter/number inside the loop we convert it to binary,
 add it to binaryImg from index of current DC-100, AND THEN DC++
 number = strtok(arguments,", /t");
 when we finish we return True and go to next line;

 6. if it is operation we need to go through few steps
 6.1 build first word:
 from operation and Absolute value (A | operation->op) we convert to value to string and we:
 6.2 go to step 7

 7.writeWord(int value, int _ARE): convert number to binary and add to binaryImg[IC-100] word and IC++;

 8.3 build second word (A | operation->funct | src reg? | src addr? || des reg? | des addr?):
 8.3.1 check operands and get values for (src/des reg and addr)
 8.3.2 parse operands, we know addr for src/des, check regs
 8.3.3 if src/des have a index or reg addresing method, we need to get registery number and convert to binary
 8.3.3.1 after we get reg number numbers ,we connect all of:
 (A | operation->funct | src reg | src addr | des reg | des addr) and build from it second word
 8.4 go to step 7

 9. if src addresing have immediate addressing method go to step 7 and pass the number
 10. if src addressing have index/direct method, get label from symbol table , check if symbol
 is type of external, if so go to step 7, pass E else pass R and base and offset.
 - if label does not exist, yield error, change state to collect errors and move next line to check if there are more errors to print, when we finish second run
 in this case, we check if globalState == collectErrors, if so, we do not export compiled files.

 11. if des addressing have index/direct do the same as in step 10

 12. if src addresing have immediate addresing method, go to step 7 with A and the number
 converted already to binary.

 13. if des addresing have immediate addresing method, do the same as step 12.

 14. we done , go to next line.




  */

#include "data.h"
  /* from variables.c (global Variables) */
extern State globalState;
extern unsigned currentLine;
extern const char* regs[REGS_SIZE];

/* from firstRun.c */
extern void parseSingleLine(char* line);
extern ParseState handleFirstToken(char* token, char* line, ParseState state);
extern Bool parseOperands(char* src, char comma, char* des, Operation* op, AddrMethodsOptions active[2]);
extern int getInstructionType(char* s);
extern Bool isOperation(char* s);
extern Bool isValidIndexParameter(char* s);
extern Bool isRegistery(char* s);
extern Bool isValidImmediateParamter(char* s);
extern Bool isComment(char* s);


/* from table.c: */
extern int getSymbolBaseAddress(char* name);
extern int getSymbolOffset(char* name);
extern Bool isExternal(char* name);
extern Item* getSymbol(char* name);

/* from operation.c */
extern Operation* getOperationByName(char* s);

/* from memory.c */
extern unsigned getDC();
extern unsigned getIC();
extern void addWord(int value, DataType type);



int secondRunParsing(FILE* fp, char* filename)
{
    int c = 0;
    int i = 0;
    char line[MAX_LINE_LEN + 1] = { 0 };
    currentLine = 1;
    initMemory();
    printf("\n\n\nSecond Run:\n");
    while (((c = fgetc(fp)) != EOF))
    {

        if (c == '\n')
        {
            parseSingleLinesecondRunParsing(line);
            memset(line, 0, MAX_LINE_LEN);
            i = 0;
        }

        else if (isspace(c))
            line[i++] = ' ';

        else
        {
            if (isprint(c))
                line[i++] = c;
        }
    }

    if (i > 0)
    {
        parseSingleLinesecondRunParsing(line);
        memset(line, 0, i);
    }

    return True;
}
void parseSingleLinesecondRunParsing(char* line)
{
    ParseState state = newLine;
    char lineCopy[MAX_LINE_LEN] = { 0 };
    char* token;
    memcpy(lineCopy, line, MAX_LINE_LEN);
    printf("\nLine Number (%d):\n%s\n", currentLine, line);
    token = strtok(lineCopy, ", \t \n");

    while (token != NULL && state != lineParsedSuccessfully)
    {
        state = handleState(token, line, state);
        switch (state)
        {
        case lineParsedSuccessfully:
        {
            break;
        }
        case skipToNextToken:
        {
            line = line + strlen(token) + 1;
            state = handleState(strtok(NULL, ", \t \n"), line, newLine);
            break;
        }
        case Err:
        {
            globalState = collectErrors;
            break;
        }
        default:
            break;
        }
        token = strtok(NULL, ", \t \n");
    }

    currentLine++;
}

ParseState handleState(char* token, char* line, ParseState state)
{

    switch (state)
    {
    case skipLine:
        return lineParsedSuccessfully;

    case newLine:
    {
        if (isComment(token))
            return lineParsedSuccessfully;

        if (isLabel(token))
            return skipToNextToken;


        else if (isInstruction(token))
        {
            int type = getInstructionType(token);

            if (type == _TYPE_DATA)
                return writeDataInstruction(strtok(NULL, ", \t \n"));
            else if (type == _TYPE_STRING)
                return writeStringInstruction(strtok(NULL, ", \t \n"));
            else
                return lineParsedSuccessfully;
        }

        else if (isOperation(token))
        {
            char args[MAX_LINE_LEN] = { 0 };
            line = line + strlen(token);
            strcpy(args, line);
            return writeOperationBinary(token, line);
        }
    }

    default:
        break;
    }
    return True;
}

Bool writeOperationBinary(char* operationName, char* args)
{
    Operation* op = getOperationByName(operationName);
    char* first, * second;
    AddrMethodsOptions active[2] = { {0, 0, 0, 0}, {0, 0, 0, 0} };
    first = strtok(args, ", \t \n");
    second = strtok(NULL, ", \t \n");

    writeFirstWord(op);
    if (first && second && (detectOperandType(first, active, 0) && detectOperandType(second, active, 1)))
    {
        /*         printf("operation:%s args:%s first:%s second:%s\n", operationName, args, first, second);
         */
        writeSecondWord(first, second, active, op);

        if (active[0].index)
        {
            parseLabelNameFromIndexAddrOperand(first);
            writeDirectOperandWord(first);
        }
        else if (active[0].direct)
            writeDirectOperandWord(first);
        else if (active[0].immediate)
            writeImmediateOperandWord(first);

        if (active[1].direct)
            writeDirectOperandWord(second);

        else if (active[1].immediate)
            writeImmediateOperandWord(second);

        else if (active[1].index)
        {
            parseLabelNameFromIndexAddrOperand(second);
            writeDirectOperandWord(second);
        }
    }
    else if (!second && first && detectOperandType(first, active, 1))
    {

        second = first;
        /*
                printf("operation:%s args:%s first:%s second:%s\n", operationName, args, first, second);
         */
        writeSecondWord(first, second, active, op);

        if (active[1].index)
        {
            parseLabelNameFromIndexAddrOperand(second);
            writeDirectOperandWord(second);
        }
        else if (active[1].direct)
            writeDirectOperandWord(second);
        else if (active[1].immediate)
            writeImmediateOperandWord(second);
    }
    else if (!first && !second && !op->funct)
        return lineParsedSuccessfully;

    else
        return Err;

    return lineParsedSuccessfully;
}

Bool writeDataInstruction(char* token)
{
    int num;
    while (token != NULL)
    {
        num = atoi(token);
        addWord((A << 16) | num, Data);
        token = strtok(NULL, ", \t \n");
    }
    return lineParsedSuccessfully;
}

Bool writeStringInstruction(char* s)
{
    int i = 1;
    /*
     printf("inside write String Instruction, token: %s\n", s);
  */
    for (i = 1; s[i] != '\"' && s[i] != '\0'; i++)
        addWord((A << 16) | s[i], Data);
    addWord((A << 16) | '\0', Data);
    return lineParsedSuccessfully;
}

void writeSecondWord(char* first, char* second, AddrMethodsOptions active[2], Operation* op)
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

void writeFirstWord(Operation* op)
{
    unsigned firstWord = (A << 16) | op->op;
    addWord(firstWord, Code);
}

void writeDirectOperandWord(char* labelName)
{
    unsigned base = 0, offset = 0;
    int _ARE = isExternal(labelName) ? E : R;
    base = getSymbolBaseAddress(labelName);
    offset = getSymbolOffset(labelName);
    addWord((_ARE << 16) | base, Code);
    addWord((_ARE << 16) | offset, Code);
}

void writeImmediateOperandWord(char* n)
{
    n++;
    addWord((A << 16) | atoi(n), Code);
}

Bool detectOperandType(char* operand, AddrMethodsOptions active[2], int type)
{
    if (isRegistery(operand))
        active[type].reg = 1;
    else if (isValidImmediateParamter(operand))
        active[type].immediate = 1;
    else if (isValidIndexParameter(operand))
        active[type].index = 1;
    else
    {
        if (getSymbol(operand) != NULL)
            active[type].direct = 1;
        else
            return yieldError(labelNotExist);
    }
    return True;
}

char* parseLabelNameFromIndexAddrOperand(char* s)
{
    char* p = strchr(s, '[');
    *p = 0;
    return s;
}

int parseRegNumberFromIndexAddrOperand(char* s)
{
    char* p = strchr(s, ']');
    s = strchr(s, '[');
    s++;
    if (p) *p = 0;
    return getRegisteryNumber(s);
}