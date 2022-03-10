/*

Symbol table is fully updated with all of the symbols with correct
values (.data/.string/.entry arguments address is updated as well to start after last
operation address)


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

/* from table.c: */
extern int getSymbolBaseAddress(char* name);
extern int getSymbolOffset(char* name);
extern Bool isExternal(char* name);

/* from operation.c */
extern Operation operations[OP_SIZE];
extern Operation* getOperationByName(char* s);

/* from memory.c */
extern unsigned getDC();
extern unsigned getIC();
extern void writeIntegerIntoDataMemoryBinaryImg(int number);
extern void writeIntoDataBinaryImg(char s[BINARY_WORD_SIZE]);
extern void writeIntoCodeBinaryImg(char s[BINARY_WORD_SIZE]);

/* from encode.c */
extern char* generateFirstWordEncodedToBinary(Operation* operation);






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



void addWord(int value, int _ARE) {
    int wordDecimal = _ARE | value;
    char wordBinary[BINARY_WORD_SIZE] = { 0 };
    strncpy(wordBinary, hexToBin(decToHex(wordDecimal)), BINARY_WORD_SIZE);
    printf("word binary string: %s\n", wordBinary);
}







int secondRunParseSource(FILE* fp, char* filename)
{
    int c = 0;
    int i = 0;
    char line[MAX_LINE_LEN + 1] = { 0 };
    currentLine = 1;
    initMemory();

    printf("Second Run:\n");
    while (((c = fgetc(fp)) != EOF))
    {

        if (c == '\n')
        {
            parseSingleLineSecondRun(line);
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
        parseSingleLine(line);
        memset(line, 0, i);
    }

    return True;
}
void parseSingleLineSecondRun(char* line)
{
    ParseState state = newLine;
    char* p = calloc(strlen(line + 1), sizeof(char*));
    /* char* token = calloc(MAX_LABEL_LEN, sizeof(char*)); */
    char* token;
    printf("\ninside parseSingleLine, Line Number (%d):\n%s\n", currentLine, line);
    memcpy(p, line, strlen(line));
    token = strtok(p, ", \t \n");

    while (token != NULL && state != lineParsedSuccessfully)
    {
        printf("inside while loop, token:%s\n", token);
        state = handleSecondRunFirstToken(token, p, state);

        switch (state)
        {

        case writingOperationIntoMemoryImg:
        {
            printf("state: write operation\n");


            /*
            state = writeOperationBinary(token, line);
 */

            break;
        }

        case writingDataIntoMemoryImg:
        {
            printf("state: write instruction\n");

            /* state = writeInstructionBinary(token, line);*/
            break;
        }
        case lineParsedSuccessfully: {
            break;

        }
        case skipToNextToken: {
            break;
        }
        case Err:
        {
            if (globalState == firstRun)
                globalState = collectErrors;

            state = Err;
            break;
        }


        default:
            break;

        }

        token = strtok(NULL, ", \t \n");
    }

    if (p != NULL) { free(p); }

    currentLine++;


}

ParseState handleSecondRunFirstToken(char* token, char* line, ParseState state)
{


    switch (state)
    {
    case skipLine:
        return lineParsedSuccessfully;

    case newLine:
    {

        if (token[0] == ';')
            return lineParsedSuccessfully;

        if (isLabel(token))
            return skipToNextToken;

        else if (isInstruction(token))
        {
            int type = getInstructionType(token);

            printf("line 249, if(isInstruction)\n");
            if (type == _TYPE_DATA || type == _TYPE_STRING)
                return writingDataIntoMemoryImg;
            else
                return lineParsedSuccessfully;
        }

        else if (isOperation(token))
        {
            printf("line 255, token is operation\n");
            return writingOperationIntoMemoryImg;
        }
    }

    default:
        break;
    }
    return True;
}

Bool writeOperationBinary(char* operationName, char* line)
{
    Operation* p = getOperationByName(operationName);
    char firstOperand[MAX_LABEL_LEN] = { 0 }, secondOperand[MAX_LABEL_LEN] = { 0 };
    char comma = 0;
    int nTotal = 0, nFirst = 0;
    AddrMethodsOptions active[2] = { {0, 0, 0, 0}, {0, 0, 0, 0} };
    printf("writeOperationBinary in second run line 84\n");
    line = operationName + strlen(operationName) + 1;
    sscanf(line, "%s%n%c%s%n", firstOperand, &nFirst, &comma, secondOperand, &nTotal);
    if (secondOperand[0] == 0 && firstOperand[0] != 0)
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

    if (parseOperands(firstOperand, comma, secondOperand, p, active))
    {

        unsigned srcBase = 0, srcOffset = 0, desBase = 0, desOffset = 0;
        printf("line 183, handle Operation\n");
        printf("active:\nSRC: direct:%u index:%u immediate:%u reg:%u\n", active[0].direct, active[0].index, active[0].immediate, active[0].reg);
        printf("DES: direct:%u index:%u immediate:%u reg:%u\n", active[1].direct, active[1].index, active[1].immediate, active[1].reg);

        writeFirstWord(p);

        if (active[0].direct || active[0].index)
        {

            srcBase = getSymbolBaseAddress(firstOperand);
            srcOffset = getSymbolOffset(firstOperand);

            if (!srcBase || !srcOffset)
            {
                globalState = secondRunFailed;
                return yieldError(labelNotExist);
            }
            else
                writeDirectOperand(srcBase, srcOffset, isExternal(firstOperand) ? E : R);
        }
        if (active[1].direct || active[1].index)
        {
            desBase = getSymbolBaseAddress(secondOperand);
            desOffset = getSymbolOffset(secondOperand);
            if (!desBase || !desOffset)
            {
                globalState = secondRunFailed;
                return yieldError(labelNotExist);
            }

            else
                writeDirectOperand(desBase, desOffset, isExternal(firstOperand) ? E : R);
        }
    }

    return True;
}

void writeDirectOperand(unsigned base, unsigned offset, int _ARE)
{

    writeIntoCodeBinaryImg(strcat(hexToBin(decToHex(_ARE)), hexToBin(decToHex(base))));
    writeIntoCodeBinaryImg(strcat(hexToBin(decToHex(_ARE)), hexToBin(decToHex(offset))));
}

void writeFirstWord(Operation* operation)
{

    writeIntoCodeBinaryImg(generateFirstWordEncodedToBinary(operation));
}

void writeSecondWord()
{

    char binaryString[BINARY_WORD_SIZE] = { "00000000000000000000" };

    writeIntoCodeBinaryImg(binaryString);
}

Bool writeInstructionBinary(char* instructionName, char* line)
{
    printf("writeInstructionBinary in second run line 170\n");

    return True;
}

